//
// Created by Ajay on 29-11-2017.
//

#ifndef POCKETDB_ENGINE_MANAGER_H
#define POCKETDB_ENGINE_MANAGER_H

#include <stdint.h>
#include "treemap.h"

void screen_initialize();
int get_user_command();
int handle_insert(treemap table);
void handle_search(treemap table, char *path);
void generate_file_name(char *file_name);
void handle_table_dump(treemap table);
static int str_comp(const void * a, const void * b);
void get_file_list(char *dirpath, char *file_list[], int *dir_len);
void handle_merge();
void pocketdb_operations();

#endif //POCKETDB_ENGINE_MANAGER_H
