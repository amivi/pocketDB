//
// Created by Ajay on 29-11-2017.
//
#include "merger.h"
#include "treemap.h"
#include "disk_level.h"
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

void screen_initialize() {
    printf("%60sPOCKET-DB\n%59s+---------+","","");
    printf("\n\n| %-20s --> 1 | %-20s --> 2| %-20s --> 3 | %-20s --> 4 | %-20s --> 5 | %-20s --> 6 |\n","CREATE NEW TABLE","INSERT DATA","SEARCH KEY","DUMP TABLE","FILE MERGER","QUIT");
}

int get_user_command() {
    int cmd;

    printf("\nYOUR INPUT : ");
    scanf("%d",&cmd);

    return cmd;
}

treemap create_new_table() {
    unsigned char table_name[255];
    treemap table1;

    printf("Enter the table name : ");
    scanf("%s", table_name);

    table1 = new_tree_map();

    return table1;
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

void handle_insert(treemap table) {
    cellptr row_data_root;
    int num_rows = 0;
    int i = 0;
    char key[FILE_KEY_LEN + 1];

    printf("\n\nEnter number of rows : ");
    scanf("%d", &num_rows);

    while (i < num_rows) {//
        printf("\n\nRow %d ::>\n", ++i);
        printf("Enter key : ");
        scanf("%s",key);
        row_data_root = row_input();
        table = tree_put(table, key, row_data_root);
    }


    printf("\n\n%s\n %d rows inserted :)\n%s\n\n", "+---------------------+", num_rows, "+---------------------+");
}

//////??????add indent to print while taking input
void handle_search(treemap table, char *path) {
    int user_cmd = 0;
    int status;
    char first_key[FILE_KEY_LEN+1];
    char last_key[FILE_KEY_LEN+1];
    char *file_list[15];
    int dir_len=0;

    printf("\n\n| %-20s --> 7 | %-20s --> 8| %-20s --> 9 |\n","SEARCH KEY","SEARCH IN RANGE","GET ALL KEYS");

    user_cmd = get_user_command();

    switch (user_cmd) {
        case 7:
            printf("\nEnter search key : ");
            scanf("%s", first_key);
            status = tree_get(table, first_key);

            if (status == EXIT_FAILURE) {
                dir_len=0;
                get_file_list(path, file_list, &dir_len);

                while (--dir_len>=2) {
                    disk_level_get(file_list[dir_len], first_key);
                }
            }
            break;
        case 8:
            printf("\nEnter first key : ");
            scanf("%s", first_key);
            printf("\nEnter last key : ");
            scanf("%s", last_key);
            tree_get_in_range(table->root,first_key,last_key);////////////////check or its okay?????

            dir_len=0;
            get_file_list(path, file_list, &dir_len);
            while (--dir_len>=2) {
                disk_level_get_in_range(file_list[dir_len], first_key, last_key);
            }
            break;
        case 9:
            tree_get_all(table->root);
            dir_len=0;
            get_file_list(path, file_list, &dir_len);
            while (--dir_len>=2) {
                disk_level_get_all(file_list[dir_len], "~");
            }
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

    /*strcpy(file_name, (char*)(tc->tm_year + 1900));
    strcat(file_name, (char*)tc->tm_mon);
    strcat(file_name, (char*)tc->tm_mday);
    strcat(file_name, (char*)tc->tm_hour);
    strcat(file_name, (char*)tc->tm_min);
    strcat(file_name, (char*)tc->tm_sec);*/
}


///?????add print to convey ops. finished
void handle_table_dump(treemap table) {
    char file_name[15];

    generate_file_name(file_name);

    disk_level_push(table, file_name);
}

void handle_merge() {
    char *file_list[15];
    char *dirpath1 = "G:\\ClionProjects\\POCKETFILES\\";
    int dir_len;
   //chaeck for less than 1 file avail
    get_file_list(dirpath1, file_list, &dir_len);

    file_merger(file_list[2], file_list[3]);
}

void pocketdb_operations() {
    static int count = 0;
    int user_command = 0;
    int quit = 0;
    treemap table1 = NULL;
    char path[100] = "G:\\ClionProjects\\POCKETFILES\\";

    screen_initialize();

    while (!quit) {

        user_command = get_user_command();
        //check for the first time input
        if (count == 0 && user_command != 1 && user_command != 6) {
            printf("\n\nOops! create a table first.");
        } else {
            count = 1;
            switch (user_command) {
                case 1:
                    table1 = create_new_table();
                    if (!table1) {
                        printf("\n\nError: new table was not created !\n\n");
                    } else {
                        printf("\n\nTable created.\n\n");
                    }
                    break;
                case 2:
                    handle_insert(table1);
                    break;
                case 3:
                    handle_search(table1, path);
                    break;
                case 4:
                    handle_table_dump(table1);
                    break;
                case 5:
                    handle_merge();
                    break;
                case 6:
                    quit = 1;
                    break;
                default:
                    printf("\n\n:( Invalid choice. Try again !\n\n");
            }
        }
    }
}
