//
// Created by Ajay on 29-11-2017.
//
#include "merger.h"
#include "treemap.h"
#include "disk_level.h"
#include "path.h"
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

void screen_initialize() {
    int opt;
    printf("%60sPOCKET-DB\n%59s+---------+","","");
    char *option_menu[] = {"INSERT DATA","SEARCH KEY","DUMP TABLE","FILE MERGER","QUIT"};

    printf("\n");
    for (opt=0; opt<5; opt++)
        printf("\n|| %-20s --> %d ||",option_menu[opt],opt+1);
    printf("\n\n");
}

int get_user_command() {
    int cmd;

    printf("\nYOUR INPUT : ");
    scanf("%d",&cmd);

    return cmd;
}

static int str_comp(const void * a, const void * b) {
    return strcmp(*(const char **) a, *(const char **) b);
}

void get_file_list(char *dirpath, char *file_list[], int *dir_len) {
    int i=0;
    DIR *d;
    struct dirent *dir;

    d = opendir(dirpath);
    if (d)
    {
        for (i=0;(dir = readdir(d)) != NULL; i++)
        {
            file_list[i] = malloc(sizeof(char) * 15);
            strcpy(file_list[i], dir->d_name);
        }
        closedir(d);
    }

    if (i) {
        *dir_len = i;
        qsort(file_list, (size_t) *dir_len, sizeof(const char *), str_comp);
    }
}

int handle_insert(treemap table) {
    cellptr row_data_root;
    int num_rows;
    int i;
    char key[FILE_KEY_LEN + 1];
    char key_copy[FILE_KEY_LEN+1];

    num_rows = 0;
    i = 0;
    printf("\n\nEnter number of rows : ");
    scanf("%d", &num_rows);

    while (i < num_rows) {
        printf("\n\nRow %d ::>\n", ++i);
        printf("Enter key : ");
        scanf("%s",key);
        sprintf(key_copy,"%10s",key);
        key_copy[10] = '\0';/////////////////////
        row_data_root = row_input();
        table = tree_put(table, key_copy, row_data_root);
    }

    return i;
}

void handle_search(treemap table, char *path) {
    int user_cmd = 0;
    int status;
    char first_key[FILE_KEY_LEN+1];
    char first_key_copy[FILE_KEY_LEN+1];
    char last_key[FILE_KEY_LEN+1];
    char last_key_copy[FILE_KEY_LEN+1];
    char *file_list[15];
    int dir_len=0;

    printf("\n\n|| %-20s --> 6 ||\n|| %-20s --> 7 ||\n|| %-20s --> 8 ||\n\n","SEARCH A KEY","SEARCH IN RANGE","GET ALL KEYS");

    printf("\nselect search option ==>\n");
    user_cmd = get_user_command();

    switch (user_cmd) {
        case 6:
            printf("\nEnter a 'search key' : ");
            scanf("%s", first_key);

            dir_len = 0;
            get_file_list(path, file_list, &dir_len);

            sprintf(first_key_copy, "%10s", first_key);
            first_key_copy[10] = '\0';

            status = get_key(table, first_key_copy, file_list, dir_len);

            break;
        case 7:
            printf("\nEnter 'first key' : ");
            scanf("%s", first_key);
            printf("\nEnter 'last key' : ");
            scanf("%s", last_key);

            sprintf(first_key_copy, "%10s", first_key);
            first_key[10] = '\0';
            sprintf(last_key_copy, "%10s", last_key);
            last_key[10] = '\0';

            dir_len=0;

            if (strcmp(first_key_copy, last_key_copy) > 0) {
                printf("\n\nInvalid key range. Please enter a valid key range.\n\n");
                break;
            }

            get_file_list(path, file_list, &dir_len);

            if (get_in_range(table, first_key_copy, last_key_copy, file_list, dir_len)==EXIT_SUCCESS) {
                printf("\n:)\n\n");
            }
            break;
        case 8:
            dir_len=0;
            get_file_list(path, file_list, &dir_len);
            get_all_keys(table, file_list, dir_len);
            break;
        default:
            printf("\n\n:( Invalid choice. Try again !\n\n");
    }
}

void generate_file_name(char *file_name) {
    time_t calender_time;
    struct tm *tc;

    time(&calender_time);
    tc = localtime(&calender_time);
    strftime(file_name,15,"%Y%m%d%H%M%S",tc);

}

void handle_table_dump(treemap table) {
    char file_name[15];

    generate_file_name(file_name);

    if (table->root != NULL) {
        if (disk_level_push(table, file_name) == EXIT_SUCCESS)
            printf("\nTable dump successful! File created : %s\n", file_name);
    } else
        printf("\n\nTable empty! Nothing to dump.\n\n");
}

void handle_merge() {
    char *file_list[15];
    char *dirpath1 = POCKETFILES_PATH;
    int dir_len;

    get_file_list(dirpath1, file_list, &dir_len);

    //check for less than 1 file
    if (dir_len < MIN_DIR_LEN) {/////
        printf("\n\nOops! It seems there's not enough files to merge.\n\n");
    } else {
        file_merger(file_list[2], file_list[3]);
    }
}

void pocketdb_operations() {

    int user_command = 0;
    int quit = 0;
    int inserted_rows = 0;
    treemap table1 = NULL;
    char path[100] = POCKETFILES_PATH;

    screen_initialize();
    table1 = new_tree_map();

    while (!quit) {

        if (!table1) {
            printf("\n\nError: new table was not created !\n\n");
        }

        user_command = get_user_command();

        switch (user_command) {

            case 1:
                inserted_rows = handle_insert(table1);
                printf("\n\n%s\n %d rows inserted :)\n%s\n\n", "+---------------------+", inserted_rows, "+---------------------+");
                break;
            case 2:
                handle_search(table1, path);
                break;
            case 3:
                handle_table_dump(table1);
                break;
            case 4:
                handle_merge();
                break;
            case 5:
                quit = 1;
                break;
            default:
                printf("\n\n:( Invalid choice. Try again !\n\n");
        }
    }
}

