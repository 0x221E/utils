#ifndef ARENA_ALLOC_H
#define ARENA_ALLOC_H

#include <stddef.h>
#include <stdlib.h>

// TODO: Add feature to support amortized O(1) dealloc, and reuse.

#define DEFAULT_ARENA_MEM_LEN 8*1024*20

typedef struct
{
  void* start;
  void* last;
  size_t cap;
  size_t len;
} Arena;

void arena_init(Arena* a, size_t len)
{
  a->start = malloc(len);
  a->last = a->start;
  a->cap = len;
  a->len = 0;
}

void* arena_alloc(Arena* a, size_t len)
{
  if(a->start == NULL || a->last == NULL) arena_init(a, DEFAULT_ARENA_MEM_LEN);

  void* res = NULL;

  if(a->len + len > a->cap)
  {   
    return res;
  }
  
  res = a->last;
  a->last = (char*)a->last + len;
  a->len += len;

  return res;
}

bool arena_dealloc(Arena* a, void* mem)
{
  return false; //NOT IMPLEMENTED YET//
}

void arena_reset(Arena* a)
{
  a->last = a->start;
  a->len = 0;
}

void arena_free(Arena* a)
{
  free(a->start);
  a->last = NULL;
  a->len = 0;
}

#endif
