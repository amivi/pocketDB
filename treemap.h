//
// Created by Ajay on 01-09-2017.
//

#ifndef POCKETDB_TREEMAP_H
#define POCKETDB_TREEMAP_H

#include "input.h"

#define KEY_NOT_FOUND 2
#define FLAG_KEY "~~~~~~~~~~"
typedef unsigned char *byte_array;
typedef struct Node *node;

/*
 * container structure for the tree node
 */
typedef struct Node {

    node left_child;
    node right_child;
    char *key;
    byte_array value;

}NODE;

typedef struct TreeMap *treemap;

/*
 * container structure for a treemap
 */
typedef struct TreeMap {

    int size;
    node root;

}TREEMAP;

treemap new_tree_map();
treemap tree_put(treemap tree, char *key, cellptr row_data);
node new_node(node root, char *key, byte_array value, int *size);
treemap tree_get_in_range(node tree, char *fkey, char *lkey, treemap range_tree);
int get_in_range(treemap tree, char *fkey, char *lkey, char *file_list[15], int dir_len);
void create_range_tree(treemap range_tree, char *key, byte_array value);
int get_key(treemap root, char *key, char *file_list[15], int dir_len);
node tree_get_node(node root, char *key);
void display_all_nodes(node root);
void display_treemap_node(char *key, cellptr first_cell);
int get_all_keys(treemap tree, char *file_list[15], int dir_len);
treemap tree_get_all(node tree, treemap all_key_tree);

#endif //POCKETDB_TREEMAP_H