//
// Created by Vijay on 23-10-2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disk_level.h"
#include "output.h"
#include "path.h"


//#define POCKETFILES_PATH NULL

/*
 * check if the file with given 'fname' already exists; if not
 * creates a file a new file and subsequently calls get_in_order
 * that retrieve nodes from the tree.
 */
int disk_level_push(treemap tree, char *name){

    FILE *fp;
    char *path = POCKETFILES_PATH;//"G:\\ClionProjects\\POCKETFILES\\";
    char *fname = malloc(strlen(name)+strlen(path)+1);
    strcpy(fname, path);
    strcat(fname, name);

    if ((fp = fopen(fname, "rb")) == NULL) {
        if((fp = fopen(fname, "wb"))) {
            get_in_order(tree->root, tree->size, fp);
            fclose(fp);
            tree->root = NULL;
            tree->size = 0;
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    } else {
        fclose(fp);
        printf("Error: Could not create new file with name '%s'",fname);
        return EXIT_FAILURE;
    }
}

/*
 * writes the key-data pair into the binary file pointed by the file
 * pointer fp.
 */
void handle_push(FILE *fp, char *key, byte_array data, int total_key) {

    static unsigned long int file_size_counter;
    static unsigned int bucket_size_counter;
    static unsigned int key_count;
    unsigned int data_len = 0;
    data_len = (int) data[0];               //retrieve total length of byte-array
    ++key_count;                            //keep track of number of keys written
    static long int p1, p2;

    //check for the fresh file.
    if (file_size_counter == 0) {
        fprintf(fp, "%10s", key);
        p1 = ftell(fp);
        file_size_counter += FILE_HEADER_SIZE;
        fseek(fp, FILE_HEADER_SIZE, SEEK_SET);
    }

    //check for the fresh bucket.
    if (bucket_size_counter == 0) {
        fprintf(fp, "%10s", key);
        p2 = ftell(fp);
        bucket_size_counter += BUCKET_HEADER_SIZE;
        file_size_counter += BUCKET_HEADER_SIZE;
        fseek(fp, file_size_counter, SEEK_SET);///////////
    }

    //in every case write key-data pair to the file.
    fprintf(fp, "%10s", key);
    fwrite(data, data_len, 1, fp);
    file_size_counter += data_len + FILE_KEY_LEN;
    bucket_size_counter += data_len + FILE_KEY_LEN;

    //check for the key that fills the current bucket.
    if (key_count % BUCKET_KEY_LIMIT == 0){
        //goto position of the last key in the bucket header.
        fseek(fp, p2, SEEK_SET);
        //write last key.
        fprintf(fp, "%10s", key);
        //write bucket size.
        fwrite(&bucket_size_counter, BUCKET_SIZE_BYTES, 1, fp);
        //goto the last position in the current file.
        fseek(fp, file_size_counter, SEEK_SET);//////////////////////XXXXXXXX
        //set bucket size counter to 0.
        bucket_size_counter = 0;
    }

    //check for the incoming last key to the file.
    if (key_count == total_key) {
        //avoid overwrite when 'total_key % BUCKET_KEY_LIMIT == 0'
        if (key_count % BUCKET_KEY_LIMIT != 0) {
            fseek(fp, p2, SEEK_SET);
            fprintf(fp, "%10s", key);
            fwrite(&bucket_size_counter, BUCKET_SIZE_BYTES, 1, fp);
        }
        //goto position of the last key in the file header.
        fseek(fp, p1, SEEK_SET);
        //write last key.
        fprintf(fp, "%10s", key);
        //write number of keys.
        fwrite(&key_count, FILE_NKEY_BYTES, 1, fp);
        //write file size.
        fwrite(&file_size_counter, FILE_SIZE_BYTES, 1, fp);

        file_size_counter = 0;
        bucket_size_counter = 0;
        key_count = 0;
        p1 = p2 = 0;
    }
}

/*
 * traverse the tree in 'inorder' fashion and pass the key-value
 * pair from the nodes to be written to the file pointed by file
 * pointer 'fp'.
 */
void get_in_order(node root, int size, FILE *fp){

    if (root != NULL) {
        get_in_order(root->left_child, size, fp);
        handle_push(fp, root->key, root->value, size);
        get_in_order(root->right_child, size, fp);
    }
}


int disk_level_get(char *fname, char *key){
    FILE *fp;
    char path[100]= POCKETFILES_PATH;//"G:\\ClionProjects\\POCKETFILES\\";
    char key_copy[11], current_key[11];
    char first_key[11], last_key[11];
    int num_keys;
    uint8_t data_len;
    long int file_size, file_size_track = 0, posx;
    int bucket_size_track = 0, bucket_size;
    byte_array data;

    //sprintf(key_copy, "%10s", key);
    //key_copy[10] = '\0';

    strcat(path, fname);
    fp = fopen(path, "rb");

    if (fp) {
        fread(first_key, FILE_KEY_LEN, 1, fp);
        fread(last_key, FILE_KEY_LEN, 1, fp);
        fread(&num_keys, FILE_NKEY_BYTES, 1, fp);
        fread(&file_size, FILE_SIZE_BYTES, 1, fp);
        first_key[10] = '\0';
        last_key[10] = '\0';
        file_size_track += FILE_HEADER_SIZE;

        if (strcmp(key, first_key) >= 0 && strcmp(key, last_key) <= 0) {

            while (file_size_track != file_size) {

                fread(first_key, FILE_KEY_LEN, 1, fp);
                fread(last_key, FILE_KEY_LEN, 1, fp);
                fread(&bucket_size, BUCKET_SIZE_BYTES, 1, fp);
                first_key[10] = '\0';
                last_key[10] = '\0';

                bucket_size_track += BUCKET_HEADER_SIZE;

                if (strcmp(key, first_key) >= 0 && strcmp(key, last_key) <= 0) {
                    while (bucket_size_track != bucket_size) {
                        fread(current_key, FILE_KEY_LEN, 1, fp);
                        current_key[10] = '\0';
                        posx = ftell(fp);
                        fread(&data_len, 1, 1,fp);
                        if (strcmp(current_key, key) == 0) {
                            data = malloc(sizeof(char) * ((int)data_len));
                            fseek(fp, posx, SEEK_SET);
                            fread(data, (int)data_len, 1, fp);
                            display_treemap_node(key, data_expand(data));
                            printf("\n");
                            fclose(fp);
                            return (EXIT_SUCCESS);

                        } else if (strcmp(current_key, key) > 0) {
                            fclose(fp);
                            return KEY_NOT_FOUND;
                        }
                        fseek(fp, (int)data_len - 1, SEEK_CUR);
                        bucket_size_track += (int)data_len + FILE_KEY_LEN;
                        file_size_track += (int)data_len + FILE_KEY_LEN;
                    }
                    fclose(fp);
                    return KEY_NOT_FOUND;
                }
                fseek(fp, bucket_size - BUCKET_HEADER_SIZE, SEEK_CUR);
                file_size_track += bucket_size;
            }
            return KEY_NOT_FOUND;

        } else {
            fclose(fp);
            return KEY_NOT_FOUND;
        }
    } else {
        printf("\n\nfile %s/%s can't be opened!\n\n", path, fname);
    }
}

int disk_level_get_all(char *fname, char flag_key[11], treemap range_tree){
    FILE *fp;
    char path[100]= POCKETFILES_PATH;//"G:\\ClionProjects\\POCKETFILES\\";
    char key[11];
    int num_keys;
    uint8_t data_len;
    int key_count=0;
    byte_array data;
    long int posx;

    strcat(path, fname);
    fp = fopen(path, "rb");

    key[0] = 0;
    if (fp) {
        fseek(fp, FILE_KR_BYTES, SEEK_SET);
        fread(&num_keys, FILE_NKEY_BYTES, 1, fp);
        fseek(fp, FILE_HEADER_SIZE + BUCKET_HEADER_SIZE, SEEK_SET);
        while (++key_count <= num_keys && strcmp(key, flag_key) < 0) {
            fread(key, FILE_KEY_LEN, 1, fp);
            key[10] = '\0';
            posx = ftell(fp);
            fread(&data_len, 1, 1, fp);
            data = malloc(sizeof(char) * ((int) data_len));
            fseek(fp, posx, SEEK_SET);
            fread(data, (int) data_len, 1, fp);
            create_range_tree(range_tree, key, data);

            if (key_count % BUCKET_KEY_LIMIT == 0)
                fseek(fp, BUCKET_HEADER_SIZE, SEEK_CUR);
        }
        fclose(fp);
        return EXIT_SUCCESS;
    } else {
        printf("\nError: cant open file at %s", path);
        fclose(fp);
        return EXIT_FAILURE;
    }
}

int disk_level_get_in_range(char *fname, char *fkey, char *lkey, treemap range_tree){//////addedrange tree in arg
    FILE *fp;
    char path[100] = POCKETFILES_PATH;//"G:\\ClionProjects\\POCKETFILES\\";
    char fkey_copy[11], current_key[11], lkey_copy[11];
    char first_key[11], last_key[11];
    int num_keys;
    uint8_t data_len;
    long int file_size, file_size_track = 0, posx;
    int bucket_size_track = 0, bucket_size;
    byte_array data;

    /*sprintf(fkey_copy, "%10s", fkey);
    fkey_copy[10] = '\0';
    sprintf(lkey_copy, "%10s", lkey);
    lkey_copy[10] = '\0';*/

    strcat(path, fname);
    fp = fopen(path, "rb");

    if (fp) {
        fread(first_key, FILE_KEY_LEN, 1, fp);
        fread(last_key, FILE_KEY_LEN, 1, fp);
        fread(&num_keys, FILE_NKEY_BYTES, 1, fp);
        fread(&file_size, FILE_SIZE_BYTES, 1, fp);
        first_key[10] = '\0';
        last_key[10] = '\0';
        file_size_track += FILE_HEADER_SIZE;

        if ( strcmp(fkey, first_key) >= 0 && strcmp(fkey, last_key) <= 0) {

            while (file_size_track != file_size) {
                fread(first_key, FILE_KEY_LEN, 1, fp);
                fread(last_key, FILE_KEY_LEN, 1, fp);
                fread(&bucket_size, BUCKET_SIZE_BYTES, 1, fp);
                first_key[10] = '\0';
                last_key[10] = '\0';

                bucket_size_track = BUCKET_HEADER_SIZE;

                if (strcmp(fkey, first_key) >= 0 && strcmp(fkey, last_key) <= 0) {

                    file_size_track += BUCKET_HEADER_SIZE;
                    memcpy(current_key, first_key, 11);

                    while (strcmp(current_key, fkey) < 0) {/////////////////////////
                        posx = ftell(fp);
                        fread(current_key, FILE_KEY_LEN, 1, fp);
                        current_key[10] = '\0';

                        //when previous key was smaller than first key but
                        //current key is greater than or equal to first key
                        if (strcmp(current_key, fkey) >= 0) {
                            fseek(fp, posx, SEEK_SET);
                            break;
                        } else {
                            fread(&data_len, 1, 1, fp);
                            fseek(fp, (int) data_len - 1, SEEK_CUR);
                            bucket_size_track += (int) data_len + FILE_KEY_LEN;
                            file_size_track += (int) data_len + FILE_KEY_LEN;
                        }
                    }

                    while ((strcmp(lkey, current_key) >= 0) && (file_size_track < file_size)) {

                        fread(current_key, FILE_KEY_LEN, 1, fp);

                        if (strcmp(lkey, current_key) >= 0) {
                            current_key[10] = '\0';
                            posx = ftell(fp);
                            fread(&data_len, 1, 1, fp);
                            data = malloc(sizeof(char) * ((int) data_len));
                            fseek(fp, posx, SEEK_SET);
                            fread(data, (int) data_len, 1, fp);
                            create_range_tree(range_tree, current_key, data);
                            bucket_size_track += (int) data_len + FILE_KEY_LEN;
                            file_size_track += (int) data_len + FILE_KEY_LEN;

                            if (bucket_size_track == bucket_size) {
                                fseek(fp, BUCKET_HEADER_SIZE, SEEK_CUR);
                                bucket_size_track += BUCKET_HEADER_SIZE;
                                file_size_track += BUCKET_HEADER_SIZE;
                            }
                        }
                    }
                    fclose(fp);
                    return EXIT_SUCCESS;
                }
                fseek(fp, bucket_size - BUCKET_HEADER_SIZE, SEEK_CUR);
                file_size_track += bucket_size;
            }

        } else {
            if (strcmp(fkey, first_key) < 0 && strcmp(lkey, first_key) >= 0) {
                disk_level_get_all(fname, lkey, range_tree);
                fclose(fp);
                return EXIT_SUCCESS;
            } else {
                fclose(fp);
                return KEY_NOT_FOUND;
            }
        }
    }
    fclose(fp);
    return EXIT_FAILURE;
}