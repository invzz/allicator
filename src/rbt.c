#include "rbt.h"
#include <stdio.h>
#include <assert.h>

void init_rb_tree(rb_tree *tree)
{
  tree->nil        = &tree->nodes[0];
  tree->nil->color = RBT_BLACK;
  tree->root       = tree->nil;
  tree->node_count = 1; // Reserve the first node for the sentinel nil node
}

rb_node *create_node(rb_tree *tree, void *start, size_t size)
{
  assert(tree->node_count < MAX_NODES); // Ensure we don't exceed the fixed memory pool

  rb_node *node    = &tree->nodes[tree->node_count++];
  node->data.start = start;
  node->data.size  = size;
  node->color      = RBT_RED;
  node->left       = tree->nil;
  node->right      = tree->nil;
  node->parent     = tree->nil;
  return node;
}

void left_rotate(rb_tree *tree, rb_node *x)
{
  rb_node *y = x->right;
  x->right   = y->left;
  if(y->left != tree->nil) { y->left->parent = x; }
  y->parent = x->parent;
  if(x->parent == tree->nil) { tree->root = y; }
  else if(x == x->parent->left) { x->parent->left = y; }
  else { x->parent->right = y; }
  y->left   = x;
  x->parent = y;
}

void right_rotate(rb_tree *tree, rb_node *x)
{
  rb_node *y = x->left;
  x->left    = y->right;
  if(y->right != tree->nil) { y->right->parent = x; }
  y->parent = x->parent;
  if(x->parent == tree->nil) { tree->root = y; }
  else if(x == x->parent->right) { x->parent->right = y; }
  else { x->parent->left = y; }
  y->right  = x;
  x->parent = y;
}

void rb_insert_fixup(rb_tree *tree, rb_node *z)
{
  while(z->parent->color == RBT_RED)
    {
      if(z->parent == z->parent->parent->left)
        {
          rb_node *y = z->parent->parent->right;
          if(y->color == RBT_RED)
            {
              z->parent->color         = RBT_BLACK;
              y->color                 = RBT_BLACK;
              z->parent->parent->color = RBT_RED;
              z                        = z->parent->parent;
            }
          else
            {
              if(z == z->parent->right)
                {
                  z = z->parent;
                  left_rotate(tree, z);
                }
              z->parent->color         = RBT_BLACK;
              z->parent->parent->color = RBT_RED;
              right_rotate(tree, z->parent->parent);
            }
        }
      else
        {
          rb_node *y = z->parent->parent->left;
          if(y->color == RBT_RED)
            {
              z->parent->color         = RBT_BLACK;
              y->color                 = RBT_BLACK;
              z->parent->parent->color = RBT_RED;
              z                        = z->parent->parent;
            }
          else
            {
              if(z == z->parent->left)
                {
                  z = z->parent;
                  right_rotate(tree, z);
                }
              z->parent->color         = RBT_BLACK;
              z->parent->parent->color = RBT_RED;
              left_rotate(tree, z->parent->parent);
            }
        }
    }
  tree->root->color = RBT_BLACK;
}

void rb_insert(rb_tree *tree, void *start, size_t size)
{
  rb_node *z = create_node(tree, start, size);
  rb_node *y = tree->nil;
  rb_node *x = tree->root;

  while(x != tree->nil)
    {
      y = x;
      if(z->data.start < x->data.start) { x = x->left; }
      else { x = x->right; }
    }
  z->parent = y;
  if(y == tree->nil) { tree->root = z; }
  else if(z->data.start < y->data.start) { y->left = z; }
  else { y->right = z; }
  z->left  = tree->nil;
  z->right = tree->nil;
  z->color = RBT_RED;
  rb_insert_fixup(tree, z);
}

rb_node *rb_search(rb_tree *tree, void *start)
{
  rb_node *x = tree->root;
  while(x != tree->nil && x->data.start != start)
    {
      if(start < x->data.start) { x = x->left; }
      else { x = x->right; }
    }
  return x;
}

void rb_transplant(rb_tree *tree, rb_node *u, rb_node *v)
{
  if(u->parent == tree->nil) { tree->root = v; }
  else if(u == u->parent->left) { u->parent->left = v; }
  else { u->parent->right = v; }
  v->parent = u->parent;
}

rb_node *rb_minimum(rb_tree *tree, rb_node *x)
{
  while(x->left != tree->nil) { x = x->left; }
  return x;
}

void rb_delete_fixup(rb_tree *tree, rb_node *x)
{
  while(x != tree->root && x->color == RBT_BLACK)
    {
      if(x == x->parent->left)
        {
          rb_node *w = x->parent->right;
          if(w->color == RBT_RED)
            {
              w->color         = RBT_BLACK;
              x->parent->color = RBT_RED;
              left_rotate(tree, x->parent);
              w = x->parent->right;
            }
          if(w->left->color == RBT_BLACK && w->right->color == RBT_BLACK)
            {
              w->color = RBT_RED;
              x        = x->parent;
            }
          else
            {
              if(w->right->color == RBT_BLACK)
                {
                  w->left->color = RBT_BLACK;
                  w->color       = RBT_RED;
                  right_rotate(tree, w);
                  w = x->parent->right;
                }
              w->color         = x->parent->color;
              x->parent->color = RBT_BLACK;
              w->right->color  = RBT_BLACK;
              left_rotate(tree, x->parent);
              x = tree->root;
            }
        }
      else
        {
          rb_node *w = x->parent->left;
          if(w->color == RBT_RED)
            {
              w->color         = RBT_BLACK;
              x->parent->color = RBT_RED;
              right_rotate(tree, x->parent);
              w = x->parent->left;
            }
          if(w->right->color == RBT_BLACK && w->left->color == RBT_BLACK)
            {
              w->color = RBT_RED;
              x        = x->parent;
            }
          else
            {
              if(w->left->color == RBT_BLACK)
                {
                  w->right->color = RBT_BLACK;
                  w->color        = RBT_RED;
                  left_rotate(tree, w);
                  w = x->parent->left;
                }
              w->color         = x->parent->color;
              x->parent->color = RBT_BLACK;
              w->left->color   = RBT_BLACK;
              right_rotate(tree, x->parent);
              x = tree->root;
            }
        }
    }
  x->color = RBT_BLACK;
}

void rb_delete(rb_tree *tree, rb_node *z)
{
  rb_node   *y = z;
  rb_node   *x;
  node_color y_original_color = y->color;

  if(z->left == tree->nil)
    {
      x = z->right;
      rb_transplant(tree, z, z->right);
    }
  else if(z->right == tree->nil)
    {
      x = z->left;
      rb_transplant(tree, z, z->left);
    }
  else
    {
      y                = rb_minimum(tree, z->right);
      y_original_color = y->color;
      x                = y->right;
      if(y->parent == z) { x->parent = y; }
      else
        {
          rb_transplant(tree, y, y->right);
          y->right         = z->right;
          y->right->parent = y;
        }
      rb_transplant(tree, z, y);
      y->left         = z->left;
      y->left->parent = y;
      y->color        = z->color;
    }

  if(y_original_color == RBT_BLACK) { rb_delete_fixup(tree, x); }
}

void rb_inorder(rb_tree *tree, rb_node *node)
{
  if(node != tree->nil)
    {
      rb_inorder(tree, node->left);
      printf("  %p: %zu\n", node->data.start, node->data.size);
      rb_inorder(tree, node->right);
    }
}

void rb_tree_dump(rb_tree *tree, const char *name)
{
  printf("%s:\n", name);
  rb_inorder(tree, tree->root);
}
