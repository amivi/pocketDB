//
// Created by Ajay on 23-10-2017.
//

#ifndef POCKETDB_DISK_LEVEL_H
#define POCKETDB_DISK_LEVEL_H

#include "treemap.h"

#define FILE_HEADER_SIZE (FILE_KR_BYTES + FILE_NKEY_BYTES + FILE_SIZE_BYTES)
#define FILE_KR_BYTES 20
#define FILE_KEY_LEN 10
#define FILE_SIZE_BYTES sizeof(long int)
#define FILE_NKEY_BYTES sizeof(int)
#define BUCKET_HEADER_SIZE (BUCKET_KR_BYTES + BUCKET_SIZE_BYTES)
#define BUCKET_KR_BYTES 20
#define BUCKET_SIZE_BYTES sizeof(int)
#define BUCKET_KEY_LIMIT 25

int disk_level_push(treemap tree, char *name);
void handle_push(FILE *fp,char *key, byte_array data, int size);
void get_in_order(node root, int size, FILE *fp);
int disk_level_get(char *fname, char *key);
int disk_level_get_all(char *fname, char *flag_key, treemap all_key_tree);
int disk_level_get_in_range(char *fname, char *first_key, char *last_key, treemap range_tree);

#endif //POCKETDB_DISK_LEVEL_H
