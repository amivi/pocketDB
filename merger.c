//
// Created by Minal Raj on 23/11/17.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "merger.h"
#include "path.h"


void file_merger(char *fname_old1, char *fname_old2) {

    FILE *fp1;
    FILE *fp2;
    FILE *fp3;
    char path_orig1[100] = POCKETFILES_PATH;//"G:\\ClionProjects\\POCKETFILES\\";
    char path_orig2[100] = POCKETFILES_PATH;//"G:\\ClionProjects\\POCKETFILES\\";
    char path_orig3[100] = POCKETFILES_PATH;//"G:\\ClionProjects\\POCKETFILES\\";
    char path_orig_new[100] = POCKETFILES_PATH;//"G:\\ClionProjects\\POCKETFILES\\";
    char fname_new_copy[15];
    char fname_new[19];
    char path_backup1[100] = POCKETFILES_PATH_BACKUP;//"G:\\ClionProjects\\POCKETFILES_BACKUP\\";
    char path_backup2[100] = POCKETFILES_PATH_BACKUP;//"G:\\ClionProjects\\POCKETFILES_BACKUP\\";

    printf("\n\nfile merge start....");
    strcpy(fname_new, fname_old2);//////////
    strcpy(fname_new_copy, fname_new);//////////
    strcat(fname_new, "_new");
    strcat(path_orig1, fname_old1);
    strcat(path_orig2, fname_old2);
    strcat(path_orig3, fname_new);

    //opening existing files
    printf("\n\nopening older file from  : %s",path_orig1);
    fp1 = fopen(path_orig1, "rb");
    printf("\nopening latest file from : %s",path_orig2);
    fp2 = fopen(path_orig2, "rb");

    //new file to write merged data
    fp3 = fopen(path_orig3, "wb");
    printf("\n\ncreated new merge file at location : %s",path_orig3);

    //read all key-values
    get_file_data(fp1, fp2, fp3);
    printf("\nfilled merge with merged data.");

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    strcat(path_backup1, fname_old1);
    strcat(path_backup2, fname_old2);
    strcat(path_orig_new, fname_new_copy);


    rename(path_orig1, path_backup1);
    rename(path_orig2, path_backup2);
    printf("\n\nmoving original files.\nlocation : %s\nlocation : %s\n",path_backup1,path_backup2);

    rename(path_orig3, path_orig_new);
    printf("\nrenaming %s to %s\n",path_orig3,path_orig_new);

    //remove((path_orig,fname_old1));

    printf("\nfile merge complete.\n\n");
}

void get_file_data(FILE *fp1, FILE *fp2, FILE *fp3)  {

    unsigned int key_counter1 = 0;  //number of keys read from f1
    unsigned int key_counter2 = 0;  //number of keys read from f2
    unsigned long int file_size1 = 0, file_size2 = 0, file_size_counter1 = 0, file_size_counter2 = 0;
    char k1[FILE_KEY_LEN + 1];      //store current key from f1
    char k2[FILE_KEY_LEN + 1];      //store current key from f2
    char key[FILE_KEY_LEN];         //store the smaller of both keys
    unsigned char *data;            //data(byte_array) corresponding to the smaller key
    uint8_t data_len;               //data len of the current data
    char last_key1[FILE_KEY_LEN + 1];//largest key of f1
    char last_key2[FILE_KEY_LEN + 1];//largest key of f2
    char last_key[FILE_KEY_LEN];       //largest of both the largest keys
    long int pos1, pos2, posx;      //seek back to a prev pos

    /*
     *
     */
    fseek(fp1, FILE_KEY_LEN, SEEK_SET);
    fseek(fp2, FILE_KEY_LEN, SEEK_SET);
    fread(last_key1, FILE_KEY_LEN, 1, fp1);
    last_key1[10] = '\0';           //append '\0' to use the strcmp.
    fread(last_key2, 10, 1, fp2);
    last_key2[10] = '\0';

    fseek(fp1, FILE_NKEY_BYTES, SEEK_CUR);
    fseek(fp2, FILE_NKEY_BYTES, SEEK_CUR);
    fread(&file_size1, FILE_SIZE_BYTES, 1, fp1);
    fread(&file_size2, FILE_SIZE_BYTES, 1, fp2);

    int cond = strcmp(last_key1, last_key2);

    //find the last key for the resulting file
    if (cond > 0)
        memcpy(last_key, last_key1, FILE_KEY_LEN);
    else
        memcpy(last_key, last_key2, FILE_KEY_LEN);

    //seek both file pointers to first key in the first bucket.
    fseek(fp1, FILE_HEADER_SIZE + BUCKET_HEADER_SIZE, SEEK_SET);
    fseek(fp2, FILE_HEADER_SIZE + BUCKET_HEADER_SIZE, SEEK_SET);

    file_size_counter1 = FILE_HEADER_SIZE + BUCKET_HEADER_SIZE;
    file_size_counter2 = FILE_HEADER_SIZE + BUCKET_HEADER_SIZE;

    //get keys until the last key in each file reached
    while (file_size_counter1 != file_size1 && file_size_counter2 != file_size2){
        pos1 = ftell(fp1);
        fread(k1, FILE_KEY_LEN, 1, fp1);
        k1[10] = '\0';
        pos2 = ftell(fp2);
        fread(k2, FILE_KEY_LEN, 1, fp2);
        k2[10] = '\0';

        cond = strcmp(k1, k2);

        //get smaller of key and corresponding from both files
        if (cond < 0) {
            memcpy(key, k1, FILE_KEY_LEN);
            fseek(fp2, pos2, SEEK_SET);
            posx = ftell(fp1);
            fread(&data_len, 1, 1, fp1);
            data = malloc(sizeof(char)*(int)data_len);
            //data_temp = malloc(sizeof(char)*(int)data_len-1);
            fseek(fp1, posx, SEEK_SET);
            fread(data, (int)data_len, 1, fp1);
            file_write(fp3, key, data, last_key);
            ++key_counter1;
            file_size_counter1 += (int)data_len + FILE_KEY_LEN;

        } else {
            memcpy(key, k2, FILE_KEY_LEN);
            if (cond == 0) {
                fread(&data_len, 1, 1, fp1);
                fseek(fp1, ((int)data_len - 1), SEEK_CUR);
                ++key_counter1;
                file_size_counter1 += (int)data_len + FILE_KEY_LEN;
            } else {
                fseek(fp1, pos1, SEEK_SET);
            }
            posx = ftell(fp2);
            fread(&data_len, 1, 1, fp2);
            data = malloc(sizeof(char) * ((int)data_len));
            fseek(fp2, posx, SEEK_SET);
            fread(data, (int)data_len, 1, fp2);
            file_write(fp3, key, data, last_key);
            ++key_counter2;
            file_size_counter2 += (int)data_len + FILE_KEY_LEN;
        }

        if (key_counter1 % BUCKET_KEY_LIMIT == 0 && key_counter1 != 0) {
            file_size_counter1 += BUCKET_HEADER_SIZE;
            fseek(fp1, BUCKET_HEADER_SIZE, SEEK_CUR);
        }

        if (key_counter2 % BUCKET_KEY_LIMIT == 0 && key_counter2 != 0) {
            file_size_counter2 += BUCKET_HEADER_SIZE;
            fseek(fp2, BUCKET_HEADER_SIZE, SEEK_CUR);
        }

    }

    if (file_size1 != file_size_counter1) {

        while (file_size_counter1 != file_size1) {
            pos1 = ftell(fp1);
            fread(k1, FILE_KEY_LEN, 1, fp1);
            k1[10] = '\0';
            memcpy(key, k1, FILE_KEY_LEN);
            posx = ftell(fp1);
            fread(&data_len, 1, 1, fp1);
            data = malloc(sizeof(char) * (int) data_len);
            fseek(fp1, posx, SEEK_SET);
            fread(data, (int) data_len, 1, fp1);
            file_write(fp3, key, data, last_key);
            ++key_counter1;
            file_size_counter1 += (int) data_len + FILE_KEY_LEN;

            if (key_counter1 % BUCKET_KEY_LIMIT == 0 && key_counter1 != 0) {
                file_size_counter1 += BUCKET_HEADER_SIZE;
                fseek(fp1, BUCKET_HEADER_SIZE, SEEK_CUR);
            }
        }

    }

    if (file_size2 != file_size_counter2) {

        while (file_size_counter2 != file_size2) {
            pos2 = ftell(fp2);
            fread(k2, FILE_KEY_LEN, 1, fp2);
            k2[10] = '\0';
            memcpy(key, k2, FILE_KEY_LEN);
            posx = ftell(fp2);
            fread(&data_len, 1, 1, fp2);
            data = malloc(sizeof(char) * ((int) data_len));
            fseek(fp2, posx, SEEK_SET);
            fread(data, (int) data_len, 1, fp2);
            file_write(fp3, key, data, last_key);
            ++key_counter2;
            file_size_counter2 += (int) data_len + FILE_KEY_LEN;

            if (key_counter2 % BUCKET_KEY_LIMIT == 0 && key_counter2 != 0) {
                file_size_counter2 += BUCKET_HEADER_SIZE;
                fseek(fp2, BUCKET_HEADER_SIZE, SEEK_CUR);
            }
        }
    }
}

void file_write( FILE * fp3, char *key, unsigned char *data, const char *last_key) {

    static unsigned int file_size_counter3;
    static unsigned int bucket_size_counter3;
    static unsigned int key_counter3;
    char keycomp[FILE_KEY_LEN+1], lk[FILE_KEY_LEN+1];
    static long int pos1, pos2;
    ++key_counter3;
    memcpy(keycomp, key, FILE_KEY_LEN);
    keycomp[10] = '\0';
    memcpy(lk, last_key, FILE_KEY_LEN);
    lk[10] = '\0';

    //first key ie new file
    if (key_counter3 == 1) {
        fwrite(key, FILE_KEY_LEN, 1, fp3);
        pos1 = ftell(fp3);
        fseek(fp3, FILE_HEADER_SIZE, SEEK_SET);
        file_size_counter3 += FILE_HEADER_SIZE;
    }

    // new bucket
    if (key_counter3 % BUCKET_KEY_LIMIT == 1) {
        fwrite(key, FILE_KEY_LEN, 1, fp3);
        pos2 = ftell(fp3);
        fseek(fp3, (FILE_HEADER_SIZE + BUCKET_HEADER_SIZE), SEEK_SET);

        bucket_size_counter3 += BUCKET_HEADER_SIZE;
        file_size_counter3 += BUCKET_HEADER_SIZE;
    }

    //writing key - value pair in bucket
    fwrite(key, FILE_KEY_LEN, 1, fp3);
    fwrite(data, ((int) data[0]), 1, fp3);

    file_size_counter3 += (int) data[0] + FILE_KEY_LEN;
    bucket_size_counter3 += (int) data[0] + FILE_KEY_LEN;

    //bucket full
    if (key_counter3 % BUCKET_KEY_LIMIT == 0) {
        //skip to  position of the last key in the bucket header.
        fseek(fp3, pos2, SEEK_SET);
        fwrite(last_key, FILE_KEY_LEN, 1, fp3);

        fwrite(&bucket_size_counter3, BUCKET_SIZE_BYTES, 1, fp3);

        //reset bucket size counter.
        bucket_size_counter3 = 0;
    }


    //last key reached
    if ((strcmp(keycomp, lk) == 0)) {

        //prevent overwrite in case where last key is reached
        // and simultaneously last bucket is also full.
        if (key_counter3 % BUCKET_KEY_LIMIT != 0) {
            fseek(fp3, pos2, SEEK_SET);
            fwrite(key, FILE_KEY_LEN, 1, fp3);
            fwrite(&bucket_size_counter3, BUCKET_SIZE_BYTES, 1, fp3);
        }
        //now  going back to write last key in file header
        fseek(fp3, pos1, SEEK_SET);
        fwrite(key, FILE_KEY_LEN, 1, fp3);
        //writing number of keys.
        fwrite(&key_counter3, FILE_NKEY_BYTES, 1, fp3);
        //writing file size.
        fwrite(&file_size_counter3, FILE_SIZE_BYTES, 1, fp3);
    }
}