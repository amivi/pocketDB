//
// Created by Ajay on 01-09-2017.
//

#include "input.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

cellptr row_input() {

    char col, dt[7], cellname[MAXLIMIT], data_val[MAXLIMIT];
    cellptr first_cell = NULL;

    printf("Enter number of columns : ");
    scanf("%d", &col);

    while (col-- > 0) {
        cellptr new_cell = NULL;
        new_cell = malloc(sizeof(CellData));
        printf("Supported data types : INT, FLOAT, STRING\n");
        printf("Data_Type : ");
        scanf("%s", &dt);
        printf("Cell_Name : ");
        scanf("%s", cellname);
        new_cell->cell_name = strdup(cellname);
        new_cell->cell_name_len = strlen(cellname);
        printf("Data_Value : ");
        scanf("%s", data_val);

        switch (dt[0]) {
            case 'I' :
                new_cell->dt = INT;
                new_cell->val.ival = atoi(data_val);
                break;
            case 'F' :
                new_cell->dt = FLOAT;
                new_cell->val.fval = atof(data_val);
                break;
            case 'S' :
                new_cell->dt = STRING;
                new_cell->val.sval = data_val;
                break;

            default:
                printf("\aIncorrect data type !");
                exit(EXIT_FAILURE);
        }

        new_cell->val_len = strlen(data_val);
        new_cell->next_cell = NULL;
        first_cell = linked_list(first_cell, new_cell);
    }

    return first_cell;
}

cellptr linked_list(cellptr first_cell, cellptr new_cell) {

    cellptr temp = first_cell;

    if (temp == NULL) {
        temp = new_cell;
        return temp;
    }

    while(temp->next_cell)
        temp = temp->next_cell;

    temp->next_cell = new_cell;

    return first_cell;
}

unsigned int row_length(cellptr first_cell) {

    unsigned int row_len = 0;
    cellptr rootc = first_cell;

    while (rootc) {
        row_len += 3;
        row_len += rootc->cell_name_len + rootc->val_len;
        rootc = rootc->next_cell;
    }

    return row_len;
}

unsigned char *data_compact(cellptr first_cell) {

    cellptr rootc = first_cell;
    unsigned int data_len, i;

    if (rootc)
        data_len = row_length(rootc);
    else {
        printf("\aerror : data_compact : can't compact a empty cell !");
        exit(-1);
    }

    char *data_ptr = malloc(data_len);

    if (data_ptr && rootc) {
        i = 0;
        char *copy = data_ptr;
        while (rootc && i < data_len) {
            *(copy + i++) = rootc->dt;
            *(copy + i++) = rootc->cell_name_len;
            strcpy((copy + i), rootc->cell_name);  //////////////////////////////
            i += rootc->cell_name_len;
            *(copy + i++) = rootc->val_len;
            switch (rootc->dt) {
                case INT:
                    sprintf((copy + i), "%d", rootc->val.ival);
                    break;
                case FLOAT:
                    sprintf((copy + i), "%f", (rootc->val.fval));
                    break;
                case STRING:
                    strcpy((copy + i), rootc->val.sval);
                    break;
                default:
                    printf("\aunknown error : data_compact !");
            }
            i += rootc->val_len;
            rootc = rootc->next_cell;
        }
    } else {
        printf("\aerror : data_compact : memory allocation failed !");
        exit(-1);
    }

    return data_ptr;
}