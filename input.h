//
// Created by Ajay on 01-09-2017.
//

#ifndef POCKETDB_INPUT_H
#define POCKETDB_INPUT_H

#include <stdint.h>

#define MAXLIMIT 254    //maximum limit of character input
typedef struct CELL *cellptr; //structure at line:21

enum {

    INT = 01,
    FLOAT = 02,
    STRING = 04

};

/*
 * structure to hold the user input data
 */
typedef struct CELL {

    uint8_t dt;
    uint8_t cell_name_len;
    char *cell_name;
    uint8_t val_len;
    union {

        int ival;
        double fval;
        char *sval;

    } val;
    cellptr next_cell;

}CellData;

cellptr row_input(void);
cellptr linked_list(cellptr first_cell, cellptr new_cell);
unsigned int row_length(cellptr first_cell);
unsigned char *data_compact(cellptr first_cell);

#endif //POCKETDB_INPUT_H