//
// Created by Ajay on 01-09-2017.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "treemap.h"
#include "output.h"
#include "engine_manager.h"
#include "disk_level.h"
#include "path.h"


/*
 * used to create a new tree map
 * when called, it returns the pointer
 * to an empty TREEMAP structure(treemap.h line:24)
 * of size 0 and no nodes
 */
treemap new_tree_map() {

    treemap tree = (treemap) malloc(sizeof(TREEMAP));

    if (tree) {
        tree->size = 0;
        tree->root = NULL;
    }

    return tree;
}


/*
 * takes key and value as cellptr structure(input.h line:11)
 * when called, returns pointer to TREEMAP with
 * a new node(that contains the key-value) inserted
 */
treemap tree_put(treemap tree, char *key, cellptr row_data) {

    byte_array value;

    //variable value contains the transformed value as
    // byte array returned by data_compact
    value = data_compact(row_data);

    //new node adds a node with key-and the transformed value(byte array)
    //to the tree
    if((tree->root = new_node(tree->root, key, value, &(tree->size))))
        ;

    return tree;
}


/*
 * adds a node a/c to key provided on the basis of BST
 * data structure but it changes/updates the mapping if
 * an already existing key is provided & reutrns the
 * pointer to the root node of new tree after node insertion
 */
node new_node(node root, char *key, byte_array value, int *size) {

    //cond variable stores the comparision status value
    //between provided key and key of a node
    int cond;
    if (root == NULL) {
        root = malloc(sizeof(NODE));

        //strdup returns the char pointer to the string
        //provided to it.
        root->key = strdup(key);
        root->value = (byte_array) strdup((char*)value);
        root->left_child = NULL;
        root->right_child = NULL;
        *size = *size + 1;
    } else if((cond=strcmp(key, root->key)) == 0)
            root->value = (byte_array) strdup((char*)value);
    else if(cond < 0)
        root->left_child = new_node(root->left_child, key, value, size);
    else
        root->right_child = new_node(root->right_child, key, value, size);

    return root;
}

int get_all_keys(treemap tree, char *file_list[15], int dir_len) {
    treemap all_key_tree = new_tree_map();
    int index = START_INDEX;//2

    printf("\n\nfetching all key-values ....\n\n");

    while (--dir_len >= START_INDEX) {
        disk_level_get_all(file_list[index++], FLAG_KEY, all_key_tree);
    }

    all_key_tree = tree_get_all(tree->root, all_key_tree);

    if (all_key_tree->size == 0) {
        printf("\n\nNo keys. Table empty!\n");
        return KEY_NOT_FOUND;////signal of no key found.
    } else {
        display_all_nodes(all_key_tree->root);
        printf("\n\n%d keys found.\n\n",all_key_tree->size);
        return EXIT_SUCCESS;
    }
}

/*
 * outputs all the key-value mappings from treemap.
 */
treemap tree_get_all(node root, treemap all_key_tree) {

    char key[FILE_KEY_LEN+1];

    if(root!=NULL){
        tree_get_all(root->left_child, all_key_tree);
        sprintf(key, "%10s", root->key);
        key[10] = '\0';
        create_range_tree(all_key_tree, key, root->value);
        tree_get_all(root->right_child, all_key_tree);
    }

    return all_key_tree;
}

void display_all_nodes(node root) {
    if(root!=NULL){
        display_all_nodes(root->left_child);
        display_treemap_node(root->key, data_expand(root->value));
        display_all_nodes(root->right_child);
    }
}

int get_in_range(treemap tree, char *first_key, char *last_key, char *file_list[15], int dir_len) {
    treemap range_tree = new_tree_map();
    int index=START_INDEX;//2;

    printf("\n\nfetching all key-values in key range: '%s'  --  '%s'  ....\n\n",first_key, last_key);

    //get in range from disk
    while (--dir_len >= START_INDEX)//2
        disk_level_get_in_range(file_list[index++], first_key, last_key, range_tree);

    //get in range from memory
    range_tree = tree_get_in_range(tree->root, first_key, last_key, range_tree);

    if (range_tree->size == 0) {
        printf("\nNo keys found between key range : '%s' to '%s'\n", first_key, last_key);
        return KEY_NOT_FOUND;////signal of no key found.
    } else {
        display_all_nodes(range_tree->root);
        printf("\n\n%d keys found.\n\n",range_tree->size);
        return EXIT_SUCCESS;
    }
}

void create_range_tree(treemap range_tree, char *key, byte_array value) {

    if((range_tree->root = new_node(range_tree->root, key, value, &(range_tree->size))))
        ;
}

/*
 * output all the key-value mappings from treemap within the specified range.
 */
treemap tree_get_in_range(node root, char *first_key, char *last_key, treemap range_tree) {

    //char key[FILE_KEY_LEN+1];

    if(root!=NULL) {
        tree_get_in_range(root->left_child, first_key, last_key, range_tree);

        if ((strcmp(root->key, first_key) >= 0) && (strcmp(root->key, last_key)<=0)){
            create_range_tree(range_tree, root->key, root->value);
        }
        tree_get_in_range(root->right_child, first_key, last_key, range_tree);
    }

    return range_tree;
}

/*
 * searches for a key in treemap.
 */
int get_key(treemap tree, char *key, char *file_list[15], int dir_len) {

    node tree_node = NULL;
    cellptr first_cell = NULL;
    int status=-1;

    if ((tree_node = tree_get_node(tree->root, key))) {
        printf("\n\n:) key found !\n");
        first_cell = data_expand(tree_node->value);
        display_treemap_node(key, first_cell);
        printf("\n");
        return EXIT_SUCCESS;
    }

    while (--dir_len >= START_INDEX) {
        status = disk_level_get(file_list[dir_len], key);
        if (status == EXIT_SUCCESS)
            return EXIT_SUCCESS;
    }

    printf("\n\n:( 'Key : %s' not found!\n\n", key);
    return EXIT_FAILURE;
}

/*
 * return the node containing the key from the treemap if it matches
 * the search key or returns null otherwise.
 */
node tree_get_node(node root, char *key){

    if (root == NULL || (strcmp(key, root->key) == 0))
        return root;

    if (strcmp(key, root->key) < 0)
        tree_get_node(root->left_child, key);
    else
        tree_get_node(root->right_child, key);

}

/*
 * display the key-value(s).
 */
void display_treemap_node(char *key, cellptr first_cell) {

    printf("\n\nKey : ");
    printf("%-8s", key);
    while (first_cell != NULL) {
        printf("\nDataType\tCellName\tValue");
        //printf("\n%-12d",first_cell->dt);
        switch (first_cell->dt) {
            case INT :
                printf("\n%-12s","INTEGER");
                break;
            case FLOAT :
                printf("\n%-12s","FLOAT");
                break;
            case STRING :
                printf("\n%-12s","STRING");
                break;
            default:
                printf("program error");
        }

        printf("%-12s", first_cell->cell_name);
        switch (first_cell->dt) {
            case INT :
                printf("%d", first_cell->val.ival);
                break;
            case FLOAT :
                printf("%f", first_cell->val.fval);
                break;
            case STRING :
                printf("%s", first_cell->val.sval);
                break;
            default:
                printf("program error");
        }

        first_cell = first_cell->next_cell;
    }
}