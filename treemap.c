//
// Created by Ajay on 01-09-2017.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "treemap.h"

treemap new_tree_map() {

    treemap tree = (treemap) malloc(sizeof(TREEMAP));

    if (tree) {
        tree->size = 0;
        tree->root = NULL;
    }

    return tree;
}

treemap tree_put(treemap tree, char *key, cellptr row_data) {

    byte_array value;

    value = data_compact(row_data);

    if(tree->root = new_node(tree->root, key, value))
        ++tree->size;

    return tree;
}

node new_node(node root, char *key, byte_array value) {

    int cond;
    if (root == NULL) {
        root = malloc(sizeof(NODE));
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
void tree_get_all(node root) {

    if(root!=NULL){
        tree_get_all(root->left_child);
        printf("\n%s\t%s",root->key,root->value);
        tree_get_all(root->right_child);
    }
}