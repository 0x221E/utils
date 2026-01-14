#ifndef ARENA_ALLOC_H
#define ARENA_ALLOC_H

#ifdef __cplusplus
extern "C" {
#endif
  
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
  
// TODO: Add feature to support amortized O(1) dealloc, and reuse.

#define DEFAULT_ARENA_MEM_LEN 1024*20

typedef struct
{
  void* start;
  void* last;
  size_t cap;
  size_t len;
} Arena;

#define ARENA_CONST {NULL, NULL, 0, 0}

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

  size_t len_aligned = (len + 7) & ~7; // align to 8 bytes
  
  void* res = NULL;

  if(a->len + len_aligned > a->cap)
  {   
    return res;
  }
  
  res = a->last;
  a->last = (char*)a->last + len_aligned;
  a->len += len_aligned;

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

#ifdef __cplusplus
}

template<typename T>
class ArenaAlloc
{
public:
  ArenaAlloc(size_t size = DEFAULT_ARENA_MEM_LEN)
  {
    arena_init(&m_Arena, size);
  }

  ~ArenaAlloc()
  {
    arena_free(&m_Arena);
  }

  T* allocate(size_t size)
  {
    return (T*)arena_alloc(&m_Arena, sizeof(T) * size);
  }

  void deallocate(T*, size_t) {}
  
private:
  Arena m_Arena = ARENA_CONST;
};

#endif
  
#endif
