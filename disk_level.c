//
// Created by Vijay on 23-10-2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disk_level.h"
#include "output.h"


/*
 * check if the file with given 'fname' already exists; if not
 * creates a file a new file and subsequently calls get_in_order
 * that retrieve nodes from the tree.
 */
int disk_level_push(treemap tree, char *name){

    FILE *fp;
    char *path = "G:\\ClionProjects\\POCKETFILES\\";
    char *fname = malloc(strlen(name)+strlen(path)+1);
    strcpy(fname, path);
    strcat(fname, name);

    if ((fp = fopen(fname, "rb")) == NULL) {
        if((fp = fopen(fname, "wb"))) {
            get_in_order(tree->root, tree->size, fp);
            fclose(fp);
            tree->root = NULL;//////////////////////////////////set null or not????
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
        fseek(fp, (FILE_HEADER_SIZE + BUCKET_HEADER_SIZE), SEEK_SET);
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
        fseek(fp, (bucket_size_counter - FILE_HEADER_SIZE), SEEK_CUR);
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
    }
}

/*
 * traverse the tree in 'inorder' fashion and pass the key-value
 * pair from the nodes to be written to the file pointed by file
 * pointer 'fp'.
 */
void get_in_order(node root, int size, FILE *fp){
    static int i;   //iteration variable to keep track of the recursion stack.

    if (root != NULL) {
        get_in_order(root->left_child, size, fp);
        if(++i>size)
            printf("\nerror: recursion prob\n");
        handle_push(fp, root->key, root->value, size);
        get_in_order(root->right_child, size, fp);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////

int disk_level_get(char *fname, char *key){
    FILE *fp;
    char *path= "G:\\ClionProjects\\POCKETFILES\\";
    char key_copy[11], current_key[11];
    char first_key[11], last_key[11];
    int num_keys;
    uint8_t data_len;
    long int file_size, file_size_track = 0, posx;
    int bucket_size_track = 0, bucket_size;
    byte_array data;
    cellptr first_cell;

    sprintf(key_copy, "%10s", key);
    key_copy[10] = '\0';

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

        if (strcmp(key_copy, first_key) >= 0 && strcmp(key_copy, last_key) <= 0) {

            while (file_size_track != file_size) {

                fread(first_key, FILE_KEY_LEN, 1, fp);
                fread(last_key, FILE_KEY_LEN, 1, fp);
                fread(&bucket_size, BUCKET_SIZE_BYTES, 1, fp);
                first_key[10] = '\0';
                last_key[10] = '\0';

                bucket_size_track += BUCKET_HEADER_SIZE;

                if (strcmp(key_copy, first_key) >= 0 && strcmp(key_copy, last_key) <= 0) {
                    while (bucket_size_track != bucket_size) {
                        fread(current_key, FILE_KEY_LEN, 1, fp);
                        current_key[10] = '\0';
                        posx = ftell(fp);
                        fread(&data_len, 1, 1,fp);
                        if (strcmp(current_key, key_copy) == 0) {
                            data = malloc(sizeof(char) * ((int)data_len));
                            fseek(fp, posx, SEEK_SET);
                            fread(data, (int)data_len, 1, fp);
                            first_cell = data_expand(data);
                            display_treemap_node(key, first_cell);
                            fclose(fp);
                            return (EXIT_SUCCESS);

                        } else if (strcmp(current_key, key_copy) > 0) {
                            printf("\nkey: %s not present\n", key_copy);
                            fclose(fp);
                            return (2);/////exit code
                        }
                        fseek(fp, (int)data_len - 1, SEEK_CUR);
                        bucket_size_track += (int)data_len + FILE_KEY_LEN;
                    }
                    printf("\nkey: %s not present\n", key_copy);
                    fclose(fp);
                    return (2);/////////////////
                }
                fseek(fp, bucket_size - BUCKET_HEADER_SIZE, SEEK_CUR);
                file_size_track += bucket_size;
            }

        } else {
            printf("\nresult : key not present\n");
            fclose(fp);
            return (2);///////
        }
    }
}

int disk_level_get_all(char *fname, char flag_key[11]){
    FILE *fp;
    char path[100]= "G:\\ClionProjects\\POCKETFILES\\";
    char current_key[11];
    char key[11];
    int num_keys;
    uint8_t data_len;
    int key_count=0;
    byte_array data;
    cellptr first_cell;
    long int posx;

    strcat(path, fname);
    fp = fopen(path, "rb");

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
            first_cell = data_expand(data);
            display_treemap_node(key, first_cell);

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

int disk_level_get_in_range(char *fname, char *fkey, char *lkey){
    FILE *fp;
    char path[100] = "G:\\ClionProjects\\POCKETFILES\\";
    char fkey_copy[11], current_key[11], lkey_copy[11];
    char first_key[11], last_key[11];
    int num_keys;
    uint8_t data_len;
    long int file_size, file_size_track = 0, posx;
    int bucket_size_track = 0, bucket_size;
    byte_array data;
    cellptr first_cell;

    sprintf(fkey_copy, "%10s", fkey);
    fkey_copy[10] = '\0';
    sprintf(lkey_copy, "%10s", lkey);
    lkey_copy[10] = '\0';

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

        if ( strcmp(fkey_copy, first_key) >= 0 && strcmp(fkey_copy, last_key) <= 0) {

            while (file_size_track != file_size) {
                fread(first_key, FILE_KEY_LEN, 1, fp);
                fread(last_key, FILE_KEY_LEN, 1, fp);
                fread(&bucket_size, BUCKET_SIZE_BYTES, 1, fp);
                first_key[10] = '\0';
                last_key[10] = '\0';

                bucket_size_track = BUCKET_HEADER_SIZE;

                if (strcmp(fkey_copy, first_key) >= 0 && strcmp(fkey_copy, last_key) <= 0) {

                    file_size_track += BUCKET_HEADER_SIZE;
                    memcpy(current_key, first_key, 11);

                    while (strcmp(current_key, fkey_copy) < 0) {/////////////////////////
                        fread(current_key, FILE_KEY_LEN, 1, fp);
                        current_key[10] = '\0';
                        fread(&data_len, 1, 1, fp);
                        fseek(fp, (int)data_len - 1, SEEK_CUR);
                        bucket_size_track += (int)data_len + FILE_KEY_LEN;
                        file_size_track += (int)data_len + FILE_KEY_LEN;
                    }

                    while ((strcmp(lkey_copy, current_key) >= 0) && (file_size_track < file_size)) {

                        fread(current_key, FILE_KEY_LEN, 1, fp);

                        if (strcmp(lkey_copy, current_key) >= 0) {
                            current_key[10] = '\0';
                            posx = ftell(fp);
                            fread(&data_len, 1, 1, fp);
                            data = malloc(sizeof(char) * ((int) data_len));
                            fseek(fp, posx, SEEK_SET);
                            fread(data, (int) data_len, 1, fp);
                            first_cell = data_expand(data);
                            display_treemap_node(current_key, first_cell);
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
            if (strcmp(fkey_copy, first_key) < 0 && strcmp(lkey_copy, first_key) >= 0) {
                disk_level_get_all(fname, lkey_copy);
                fclose(fp);
                return EXIT_SUCCESS;
            } else {
                printf("\nresult : key not present\n");
                fclose(fp);
                return (2);///////
            }
        }
    }
    fclose(fp);
    return EXIT_FAILURE;
}