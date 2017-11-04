//
// Created by Ajay on 04-09-2017.
//
////****JUST TO CHECK THE VALUES****////
/*
 * YOU MAY DELETE THIS FILE IF YOU DON'T NEED
 */
#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "treemap.h"


int main() {

    printf("%25sPOCKET-DB\n%24s+---------+","","");
    treemap student = new_tree_map();
    cellptr root = NULL;
    char k[90];
    int rows = 7;
    char *key[] = {"15", "12", "11", "13", "17", "16", "18"};/////////////
    for(int i=0; i<rows; i++) {
        printf("\n\nROW %d-->\n",i);
        printf("enter key: ");
        sscanf(key[i],"%s",k);
        printf("%s", k);
        root = row_input();
        student = tree_put(student, k, root);
    }

    //printf("\n\nPlease enter a search key: ");
    //scanf("%s",k);
    printf("\n\n::GETTING searched KEY-VALUE FROM 'student' DATA-STORE ->");
    tree_get(student, "16");
    printf("\n\n::GETTING KEY-VALUE(S) FROM 'student' DATA-STORE in given range ->");
    tree_get_in_range(student->root,"10","15");
    printf("\n\n::GETTING ALL KEY-VALUE(S) FROM 'student' DATA-STORE ->");
    tree_get_all(student->root);
    printf("\n\nsuccess!");

    return 0;
}