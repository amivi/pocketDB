//
// Created by Ajay on 29-11-2017.
//

#ifndef POCKETDB_MERGER_H
#define POCKETDB_MERGER_H

#include <stdio.h>

void get_file_data(FILE *fp1, FILE *fp2, FILE *fp3);
void file_write(FILE *fp3, char *key, unsigned char *data, const char *last_key);
void file_merger(char *fname1, char *fname2);

#endif //POCKETDB_MERGER_H
