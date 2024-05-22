#ifndef RBT_H
#define RBT_H

#include "config.h"

#include <stddef.h>
#include <stdbool.h>

/**
 * @file rbt.h
 * @brief Red-Black Tree header file.
 *
 * This header file provides structures and functions for managing a Red-Black Tree.
 * Red-Black Trees are self-balancing binary search trees.
 *
 * @author Andres Coronado
 */

#define MAX_NODES HEAP_SIZE /**< Maximum number of nodes in the Red-Black Tree */

typedef struct chunk {
    void *start; /**< Starting address of the memory chunk */
    size_t size; /**< Size of the memory chunk */
} chunk;

/**
 * @brief Enumeration for node color in Red-Black Tree.
 */
typedef enum { 
    RBT_RED, /**< Red color */
    RBT_BLACK /**< Black color */
} node_color;

/**
 * @brief Structure representing a node in the Red-Black Tree.
 */
typedef struct rb_node {
    chunk data; /**< Data stored in the node */
    node_color color; /**< Color of the node (red or black) */
    struct rb_node *left, *right, *parent; /**< Pointers to left child, right child, and parent nodes */
} rb_node;

/**
 * @brief Structure representing a Red-Black Tree.
 */
typedef struct {
    rb_node *root; /**< Pointer to the root node of the tree */
    rb_node *nil; /**< Sentinel node */
    rb_node nodes[MAX_NODES]; /**< Array to hold tree nodes */
    size_t node_count; /**< Number of nodes currently in the tree */
} rb_tree;

/**
 * @brief Initialize a Red-Black Tree.
 *
 * @param tree Pointer to the Red-Black Tree to initialize.
 */
void init_rb_tree(rb_tree *tree);

/**
 * @brief Create a new node for the Red-Black Tree.
 *
 * @param tree Pointer to the Red-Black Tree.
 * @param start Starting address of the memory chunk for the node.
 * @param size Size of the memory chunk for the node.
 * @return Pointer to the newly created node.
 */
rb_node *create_node(rb_tree *tree, void *start, size_t size);

/**
 * @brief Insert a node into the Red-Black Tree.
 *
 * @param tree Pointer to the Red-Black Tree.
 * @param start Starting address of the memory chunk for the node to insert.
 * @param size Size of the memory chunk for the node to insert.
 */
void rb_insert(rb_tree *tree, void *start, size_t size);

/**
 * @brief Search for a node in the Red-Black Tree.
 *
 * @param tree Pointer to the Red-Black Tree.
 * @param start Starting address of the memory chunk to search for.
 * @return Pointer to the node if found, or NULL otherwise.
 */
rb_node *rb_search(rb_tree *tree, void *start);

/**
 * @brief Delete a node from the Red-Black Tree.
 *
 * @param tree Pointer to the Red-Black Tree.
 * @param z Pointer to the node to delete.
 */
void rb_delete(rb_tree *tree, rb_node *z);

/**
 * @brief Perform an in-order traversal of the Red-Black Tree.
 *
 * @param tree Pointer to the Red-Black Tree.
 * @param node Pointer to the current node (start traversal from the root).
 */
void rb_inorder(rb_tree *tree, rb_node *node);

/**
 * @brief Dump the contents of the Red-Black Tree.
 *
 * @param tree Pointer to the Red-Black Tree.
 * @param name Name of the tree (for identification in the dump).
 */
void rb_tree_dump(rb_tree *tree, const char *name);

#endif // RBT_H
