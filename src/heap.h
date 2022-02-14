/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __HEAP_H__
#define __HEAP_H__


typedef struct
{
    int     size;
    void**  tree;
    int     (*compare) (const void* key1, const void* key2);
    void    (*destroy) (void* data);
}
Heap;


void Heap_init    (Heap* heap, int (*compare)(const void* key1, const void* key2), void (*destroy)(void* data));
void Heap_destroy (Heap* heap);
int  Heap_insert  (Heap* heap, const void* data);
int  Heap_extract (Heap* heap, void** data);

#define Heap_size(heap)    ((heap)->size)
#define Heap_parent(npos)  ((int)(((npos) - 1) / 2))
#define Heap_left(npos)    (((npos) * 2) + 1)
#define Heap_right(npos)   (((npos) * 2) + 2)


#endif
