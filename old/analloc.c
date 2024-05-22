
// Deprecated!!!

#include "analloc.h"
#include <assert.h>
#include <stdbool.h>

typedef struct {
    size_t size;
    void *start;
} chunk;

typedef struct {
    size_t count;
    chunk chunks[CHUNK_LIST_CAP];
} chunk_list;

chunk_list alloc_chunks = {0};

chunk_list freed_chunks = { 
    .count = 1, 
    .chunks = {[0]={.start = heap, .size = sizeof(heap)}}, 
};


void chunk_list_insert(chunk_list *list, void *start, size_t size) {
    assert(list->count < CHUNK_LIST_CAP);
    
    list->chunks[list->count].start = start;
    list->chunks[list->count].size = size;

    for (size_t i = list->count; i > 0 && list->chunks[i].start < list->chunks[i - 1].start; --i) {
        const chunk tmp = list->chunks[i];
        list->chunks[i] = list->chunks[i - 1];
        list->chunks[i - 1] = tmp;
    }

    list->count++;
}

void chunk_list_remove(chunk_list *list, size_t index) {
    assert(index < list->count);
    for (size_t i = index; i < list->count - 1; ++i) {
        list->chunks[i] = list->chunks[i + 1];
    }
    list->count--;
}

long chunk_list_bsearch(chunk_list *list, void *ptr) {
    long left = 0;
    long right = list->count - 1;
    chunk key = {.start = ptr};

    while (left <= right) {
        long mid = left + (right - left) / 2;
        if (list->chunks[mid].start == key.start) {
            return mid;
        }
        if (list->chunks[mid].start < key.start) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

void chunk_list_merge(chunk_list *list) {
    if (list->count == 0) {
        return;
    }

    size_t dst_index = 0;

    for (size_t src_index = 1; src_index < list->count; ++src_index) {
        chunk *current = &list->chunks[dst_index];
        chunk *next = &list->chunks[src_index];

        if (current->start + current->size == next->start) {
            // Merge adjacent chunks
            current->size += next->size;
        } else {
            // Move next chunk to the next position in the merged list
            ++dst_index;
            if (dst_index != src_index) {
                list->chunks[dst_index] = *next;
            }
        }
    }

    // Update the count to reflect the merged chunks
    list->count = dst_index + 1;
}

void chunk_list_dump(chunk_list *list, const char *name) {
    printf("%s:\n", name);
    for (size_t i = 0; i < list->count; ++i) {
        printf("  %p: %zu\n", list->chunks[i].start, list->chunks[i].size);
    }
}


void an_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    const int index = chunk_list_bsearch(&alloc_chunks, ptr);
    assert(index >= 0);
    assert(alloc_chunks.chunks[index].start == ptr);
    chunk_list_insert(&freed_chunks, alloc_chunks.chunks[index].start, alloc_chunks.chunks[index].size);
    chunk_list_remove(&alloc_chunks, (size_t)index);
    ptr = NULL;
}

void *an_alloc(size_t size) {
    if (size == 0) {
        return NULL;
    }

    // Defragmentation
    chunk_list_merge(&freed_chunks);

    for (size_t i = 0; i < freed_chunks.count; ++i) {
        const chunk c = freed_chunks.chunks[i];

        if (c.size >= size) {
            chunk_list_remove(&freed_chunks, i);

            const size_t tail_size = c.size - size;
            chunk_list_insert(&alloc_chunks, c.start, size);

            if (tail_size > 0) {
                chunk_list_insert(&freed_chunks, c.start + size, tail_size);
            }

            return c.start;
        }
    }

    return NULL;
}

void an_collect() {
    // Implement garbage collection logic here if needed
}

void alloc_dump() {
    chunk_list_dump(&alloc_chunks, "Allocated Chunks");
}

void freed_dump() {
    chunk_list_dump(&freed_chunks, "Freed Chunks");
}

void an_init() {
    alloc_chunks.count = 0;
    freed_chunks.count = 1;
    freed_chunks.chunks[0].start = heap;
    freed_chunks.chunks[0].size = sizeof(heap);
}