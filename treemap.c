//
// Created by Ajay on 01-09-2017.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "treemap.h"


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
    if(tree->root = new_node(tree->root, key, value))
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
        root->value = strdup(value);
        root->left_child = NULL;
        root->right_child = NULL;
    } else if((cond=strcmp(key, root->key)) == 0)
            root->value = strdup(value);
    else if(cond < 0)
        root->left_child = new_node(root->left_child, key, value);
    else
        root->right_child = new_node(root->right_child, key, value);

    return root;
}

//not ready for use yet
/*
 * outputs all the key-value mappings of tree
 */
void tree_get_all(node root) {

    if(root!=NULL){
        tree_get_all(root->left_child);
        printf("\n%s\t%s",root->key,root->value);
        tree_get_all(root->right_child);
    }
}