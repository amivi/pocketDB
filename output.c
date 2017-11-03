//
// Created by Ajay on 17-09-2017.
//
#include <stdio.h>
#include <stdlib.h>
#include "output.h"
#include <string.h>

cellptr data_expand(byte_array val){

    int i=1;
    int data_len; //chk
    cellptr new_cell = NULL;
    cellptr first_cell = NULL;
    char temp_array[255];

    data_len = val[0];
    while(data_len-- > 0) {

        new_cell = malloc(sizeof(CellData));
        if (new_cell == NULL) {
            printf("error: data_expand: memory allocation failed!");
            exit(EXIT_FAILURE);
        }

        new_cell->dt = (uint8_t) *(val+i++);
        new_cell->cell_name_len = (uint8_t ) *(val+i++);
        strncpy(temp_array, (char *) (val + i), new_cell->cell_name_len);//////////////////////////
        temp_array[new_cell->cell_name_len] = '\0';
        new_cell->cell_name = strdup(temp_array);
        i = i + new_cell->cell_name_len;
        new_cell->val_len = (uint8_t)*(val+i++);

        strncpy(temp_array, (char *) (val + i), new_cell->val_len);
        temp_array[new_cell->val_len] = '\0';
        i = i + new_cell->val_len;

        switch (new_cell->dt) {
            case INT:
                new_cell->val.ival = atoi(temp_array);
                break;
            case FLOAT:
                new_cell->val.fval = atof(temp_array);
                break;
            case STRING:
                new_cell->val.sval = strdup(temp_array);
                break;
            default:
                printf("\a error : data_expand!");
        }

        first_cell = linked_list(first_cell, new_cell);
        new_cell = NULL;
        free(new_cell);
    }
    return  first_cell;

}
