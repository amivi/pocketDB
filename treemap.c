//
// Created by Ajay on 01-09-2017.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "treemap.h"
#include "output.h"


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
    if((tree->root = new_node(tree->root, key, value)))
        ++tree->size;

    return tree;
}


/*
 * adds a node a/c to key provided on the basis of BST
 * data structure but it changes/updates the mapping if
 * an already existing key is provided & reutrns the
 * pointer to the root node of new tree after node insertion
 */
node new_node(node root, char *key, byte_array value) {

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
    } else if((cond=strcmp(key, root->key)) == 0)
            root->value = (byte_array) strdup((char*)value);
    else if(cond < 0)
        root->left_child = new_node(root->left_child, key, value);
    else
        root->right_child = new_node(root->right_child, key, value);

    return root;
}

/*
 * outputs all the key-value mappings from treemap.
 */
void tree_get_all(node root) {

    if(root!=NULL){
        tree_get_all(root->left_child);
        display_treemap_node(root->key, data_expand(root->value));
        tree_get_all(root->right_child);
    }
}

/*
 * output all the key-value mappings from treemap within the specified range.
 */
void tree_get_in_range(node root, char *first_key, char *last_key) {

    if(root!=NULL && (strcmp(root->key, first_key) >= 0) && (strcmp(root->key, last_key)<=0)){
        tree_get_in_range(root->left_child, first_key, last_key);
        display_treemap_node(root->key, data_expand(root->value));
        tree_get_in_range(root->right_child, first_key, last_key);
    }
}

/*
 * searches for a key in treemap.
 */
int tree_get(treemap tree, char *key) {

    node tree_node = NULL;
    cellptr first_cell = NULL;

    if ((tree_node = get_node(tree->root, key))) {
        first_cell = data_expand(tree_node->value);
        display_treemap_node(key, first_cell);
        return 1;
    } else {
        printf("Key : %s not in treemap", key);
        return -1;
    }
}

/*
 * return the node containing the key from the treemap if it matches
 * the search key or returns null otherwise.
 */
node get_node(node root, char *key){

    if (root == NULL || (strcmp(key, root->key) == 0))
        return root;

    if (strcmp(key, root->key) < 0)
        get_node(root->left_child, key);
    else
        get_node(root->right_child, key);
}

/*
 * display the key-value(s).
 */
void display_treemap_node(char *key, cellptr first_cell) {

    printf("\n\nKey : ");
    printf("%-8s", key);
    while (first_cell != NULL) {
        printf("\nDataType\tCellName\tValue");
        printf("\n%-12d",first_cell->dt);
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