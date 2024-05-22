#ifndef _ALLOC_H
#define _ALLOC_H

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

/**
 * @file analloc.h
 * @brief Memory allocation header file.
 *
 * This header file provides functions for memory allocation, freeing, garbage collection,
 * and related utility functions.
 *
 * @author Andres Coronado
 */

#define CHUNK_LIST_CAP 1024

/**
 * @def CHUNK_LIST_DUMP(list)
 * @brief Macro for dumping the contents of a chunk list.
 *
 * This macro is used to print the contents of a chunk list for debugging purposes.
 *
 * @param list Pointer to the chunk list to be dumped.
 */
#define CHUNK_LIST_DUMP(list) chunk_list_dump(list, #list)

/**
 * @def UNIMPLEMENTED
 * @brief Macro for indicating unimplemented functions.
 *
 * This macro is used to print a message indicating that a function is unimplemented
 * and aborts the program.
 */
#define UNIMPLEMENTED                                                                                                                                          \
  do {                                                                                                                                                         \
      fprintf(stderr, "[ UNIMPLEMENTED ] :: %s:%d [%s]\n", __FILE__, __LINE__, __func__);                                                                      \
      abort();                                                                                                                                                 \
  } while(0)

extern char heap[HEAP_SIZE]; /**< External declaration of the heap memory */

/**
 * @brief Allocate memory of the specified size.
 *
 * @param size Size of memory to allocate.
 * @return Pointer to the allocated memory, or NULL if allocation fails.
 */
void *an_alloc(size_t size);

/**
 * @brief Free allocated memory.
 *
 * @param ptr Pointer to the memory to be freed.
 */
void an_free(void *ptr);

/**
 * @brief Perform garbage collection to reclaim unused memory.
 */
void an_collect();

/**
 * @brief Dump the contents of the freed chunks list.
 */
void freed_dump();

/**
 * @brief Dump the contents of the allocated chunks list.
 */
void alloc_dump();

/**
 * @brief Initialize the memory allocation system.
 */
void an_init();

#endif
