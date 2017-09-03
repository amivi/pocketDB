//
// Created by Ajay on 01-09-2017.
//

#ifndef POCKETDB_TREEMAP_H
#define POCKETDB_TREEMAP_H

#include "input.h"

typedef unsigned char* byte_array;
typedef struct Node *node;

typedef struct Node {

    node left_child;
    node right_child;
    char *key;
    byte_array value;

}NODE;

typedef struct TreeMap *treemap;

typedef struct TreeMap {

    int size;
    node root;

}TREEMAP;

treemap new_tree_map();
treemap tree_put(treemap tree, char *key, cellptr row_data);
node new_node(node root, char *key, byte_array value);
void tree_get_in_range(treemap tree, char *ikey, char *fkey);
node tree_get(treemap tree, char *key);
void tree_get_all(node tree);

#endif //POCKETDB_TREEMAP_H