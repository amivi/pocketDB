//
// Created by Ajay on 01-09-2017.
//

#include "input.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
 * takes the user input for a node(one row) in per call to it;
 * in CELL structure(input.h line:24) and
 * returns the pointer to the first node of linked list
 * containing user input
 */
cellptr row_input() {

    //for the purpose of testing only
    //make changes after that
    char *cellnames[] = {"BRANCH", "NAME", "ROLL"};
    char *branchval[] = {"OSS", "CCVT", "CSF"};
    char *nameval[] = {"AJAY", "VIJAY", "MINAL"};
    char *rollval[] = {"89", "80", "91"};
    char *dtval[] = {"S", "S", "I"};
    static int j;////////////
    //temporary variables for taking the user inputs and
    //some required value calculation
    int number_of_cols, i=0;
    char dt[7], cellname[MAXLIMIT], data_val[MAXLIMIT];
    //temporary pointer variable to the CELL structure which
    //will hold the input data for a row/node
    cellptr first_cell = NULL;

    printf("\nEnter number of columns : ");
    sscanf("3","%d", &number_of_cols);//////////
    printf("%d", 3);

    while (i++ < number_of_cols) {
        cellptr new_cell = NULL;
        new_cell = malloc(sizeof(CellData));
        printf("\nColumn %d :::",i-1);
        printf("\nSupported data types : INT(I), FLOAT(F), STRING(S)\n");
        printf("DataType : ");
        sscanf(dtval[i-1],"%s", dt); /////////////
        printf("%s\n", dtval[i-1]);
        printf("Cell_Name : ");
        sscanf(cellnames[i-1],"%s", cellname);////////////
        printf("%s\n", cellnames[i-1]);

        //strdup returns the char pointer to the string
        //provided to it.
        new_cell->cell_name = strdup(cellname);
        new_cell->cell_name_len = (uint8_t) strlen(cellname);
        printf("Data_Value : ");
        switch (i-1) {
            case 0: sscanf(branchval[j%3], "%s", data_val);
                printf("%s\n", branchval[j%3]);
                break;
            case 1: sscanf(nameval[j%3], "%s", data_val);
                printf("%s\n", nameval[j%3]);
                break;
            case 2: sscanf(rollval[j%3], "%s", data_val);
                printf("%s\n", rollval[j%3]);
                break;
            default: printf("\n\aerror.");
                exit(EXIT_FAILURE);
        }

        //checks for the data type of the cell data
        //and to input data to corresponding type field
        //in the union of CellData
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
                new_cell->val.sval = strdup(data_val);
                break;

            default:
                printf("\aIncorrect data type !");
                exit(EXIT_FAILURE);
        }

        new_cell->val_len = (uint8_t) strlen(data_val);
        new_cell->next_cell = NULL;

        /*
         * value returned to the first_cell is the address of
         * first node of the linked list data
         */
        first_cell = linked_list(first_cell, new_cell);
    }
    ++j;
    //pointer to the first node in the linked list
    return first_cell;
}


/*
 * inserts a new_cell node in the linked list whose
 * first node is first_cell and returns the pointer to the
 * first node in the linked list after new node insertion
 */
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

/*
 * returns the length of data contained in a linked list
 * this length does not includes padding, white spaces or
 * null characters
 */
unsigned int row_length(cellptr first_cell) {

    unsigned int row_len = 1;////////////////////////////////1
    cellptr rootc = first_cell;

    while (rootc) {
        //row_len will at-least be 3 for each cell of
        // a row in order to
        //store 1B data-type, 1B cell-name-len &
        //1B for cell-data-length
        row_len += 3;
        row_len += rootc->cell_name_len + rootc->val_len;
        rootc = rootc->next_cell;
    }

    return row_len;
}


/*
 * converts and compacts the data of linked-list
 * of CELL structures into a byte array
 * of data of length as returned by row_length;
 * and returns the byte array
 */
unsigned char *data_compact(cellptr first_cell) {

    //pointer to the first cell in the linked list
    cellptr rootc = first_cell;
    unsigned int data_len, i, cols = 0;

    //conditional to check if linked list has at-least 1 node
    //if true then gets the length of data linked list contains
    if (rootc)
        data_len = row_length(rootc);
    else {
        printf("\aerror : data_compact : can't compact a empty cell !");
        exit(EXIT_FAILURE);
    }

    char *data_ptr = malloc(data_len);

    //conditional to check if data pointer is allocated
    //a valid empty memory location
    if (data_ptr == NULL) {
        printf("\aerror : data_compact : memory allocation failed !");
        exit(EXIT_FAILURE);
    }

    //iteration variable to store the length iterated
    i = 1;/////////////////////////////////////////////////////////////////2

    //until end of the linked list is reached
    //and iteration var is smaller than data length
    while (rootc && i < data_len) {
        *(data_ptr + i++) = rootc->dt;
        *(data_ptr + i++) = rootc->cell_name_len;
        strcpy((data_ptr + i), rootc->cell_name);
        i += rootc->cell_name_len;
        *(data_ptr + i++) = rootc->val_len;
        switch (rootc->dt) {
            case INT:
                sprintf((data_ptr + i), "%d", rootc->val.ival);
                break;
            case FLOAT:
                sprintf((data_ptr + i), "%f", (rootc->val.fval));
                break;
            case STRING:
                strcpy((data_ptr + i), rootc->val.sval);
                break;
            default:
                printf("\aunknown error : data_compact !");
        }
        i += rootc->val_len;
        ++cols;
        rootc = rootc->next_cell;
    }
    *(data_ptr + 0) = (uint8_t) cols;

    return (unsigned char*) data_ptr;
}