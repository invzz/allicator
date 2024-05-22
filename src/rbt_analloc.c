#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "rbt.h"
#include "analloc.h"

rb_tree alloc_chunks;
rb_tree freed_chunks;

void an_init()
{
  init_rb_tree(&alloc_chunks);
  init_rb_tree(&freed_chunks);
  rb_insert(&freed_chunks, heap, HEAP_SIZE);
}

void *an_alloc(size_t size)
{
  if(size == 0) { return NULL; }

  // Find the first chunk that is large enough
  rb_node *x        = freed_chunks.root;
  rb_node *best_fit = freed_chunks.nil;
  while(x != freed_chunks.nil)
    {
      if(x->data.size >= size)
        {
          best_fit = x;
          x        = x->left;
        }
      else { x = x->right; }
    }

  if(best_fit == freed_chunks.nil)
    {
      return NULL; // No suitable chunk found
    }

  // Remove the best-fit chunk from freed_chunks
  rb_delete(&freed_chunks, best_fit);

  // Split the chunk if necessary
  void  *start          = best_fit->data.start;
  size_t remaining_size = best_fit->data.size - size;
  if(remaining_size > 0) { rb_insert(&freed_chunks, start + size, remaining_size); }

  // Insert the allocated chunk into alloc_chunks
  rb_insert(&alloc_chunks, start, size);
  return start;
}

// In-order traversal to get nodes in ascending order
void inorder_traversal(rb_node *node, rb_tree *tree, rb_node **prev)
{
  if(node == tree->nil) return;
  inorder_traversal(node->left, tree, prev);
  if(*prev != NULL && ((*prev)->data.start + (*prev)->data.size == node->data.start))
    {
      (*prev)->data.size += node->data.size;
      rb_delete(tree, node);
    }
  else { *prev = node; }
  inorder_traversal(node->right, tree, prev);
}

void defragment_free_list(rb_tree *tree)
{
  rb_node *node = tree->root;
  rb_node *prev = NULL;
  inorder_traversal(tree->root, tree, &prev);
}

void an_free(void *ptr)
{
  if(ptr == NULL) { return; }

  rb_node *node = rb_search(&alloc_chunks, ptr);
  assert(node != alloc_chunks.nil);
  size_t size = node->data.size;

  // Remove the chunk from alloc_chunks
  rb_delete(&alloc_chunks, node);

  // Insert the chunk into freed_chunks
  rb_insert(&freed_chunks, ptr, size);

  // Defragment the free list
  defragment_free_list(&freed_chunks);
}

void alloc_dump() { rb_tree_dump(&alloc_chunks, "Allocated Chunks"); }

void freed_dump() { rb_tree_dump(&freed_chunks, "Freed Chunks"); }

void an_collect()
{
  UNIMPLEMENTED; // Implement garbage collection logic here if needed
}