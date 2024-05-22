#include "analloc.h"
#include <stdbool.h>
#define N 10

void *ptrs[N]         = {0};
char  heap[HEAP_SIZE] = {0};

bool test_alloc()
{
  // allocating chunks
  for(int i = 0; i < N; i++) { ptrs[i] = an_alloc(i); }

  // freeing and fragmenting the heap
  for(int i = 0; i < N; i++)
    {
      if(i % 2 == 0) an_free(ptrs[i]);
    }

  // should reuse the freed chunks
  an_alloc(2);
  an_alloc(4);

  printf("\n");
  alloc_dump();
  printf("\n");
  freed_dump();
  return true;
}

int main()
{
  an_init();
  test_alloc();
  return 0;
}