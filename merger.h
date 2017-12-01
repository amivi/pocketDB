//
// Created by Ajay on 29-11-2017.
//

#ifndef POCKETDB_MERGER_H
#define POCKETDB_MERGER_H

#include <stdio.h>

#define FILE_HEADER_SIZE (FILE_KR_BYTES + FILE_NKEY_BYTES + FILE_SIZE_BYTES)
#define FILE_KR_BYTES 20
#define FILE_KEY_LEN 10
#define FILE_SIZE_BYTES sizeof(long int)
#define FILE_NKEY_BYTES sizeof(int)
#define BUCKET_HEADER_SIZE (BUCKET_KR_BYTES + BUCKET_SIZE_BYTES)
#define BUCKET_KR_BYTES 20
#define BUCKET_SIZE_BYTES sizeof(int)
#define BUCKET_KEY_LIMIT 25

void get_file_data(FILE *fp1, FILE *fp2, FILE *fp3);
void file_write(FILE *fp3, char *key, unsigned char *data, const char *last_key);
void file_merger(char *fname1, char *fname2);

#endif //POCKETDB_MERGER_H
