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
    printf("Hello, World!\n");

    treemap student = new_tree_map();
    cellptr root = NULL;
    char k[90];
    int rows = 3;
    for(int i=0; i<rows; i++) {
        printf("enter key: ");
        scanf("%s",k);
        root = row_input();
        student = tree_put(student, k, root);
    }
    tree_get_all(student->root);
    printf("\asuccess!");
    return 0;
}