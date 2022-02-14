/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "heap.h"

#include <stdlib.h>
#include <string.h>



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void Heap_init(Heap* heap, int (*compare)(const void*, const void*), void (* destroy)(void* data))
{
    heap->size    = 0;
    heap->compare = compare;
    heap->destroy = destroy;
    heap->tree    = NULL;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
void Heap_destroy(Heap* heap)
{
    int i;

    if (heap->destroy != NULL)
    {
        for (i = 0; i < Heap_size(heap); i++)
        {
            heap->destroy(heap->tree[i]);
        }
    }

    free(heap->tree);

    memset(heap, 0, sizeof(Heap));
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
int Heap_insert(Heap* heap, const void* data)
{
    void* temp;
    int ipos;
    int ppos;

    temp = (void**) realloc(heap->tree, sizeof(void*) * (Heap_size(heap) + 1));
    if (temp == NULL)
    {
        return -1;
    }
    else
    {
        heap->tree = (void**) temp;
    }

    heap->tree[Heap_size(heap)] = (void*)data;

    ipos = Heap_size(heap);
    ppos = Heap_parent(ipos);

    while (ipos > 0 && heap->compare(heap->tree[ppos], heap->tree[ipos]) < 0)
    {
        temp = heap->tree[ppos];
        heap->tree[ppos] = heap->tree[ipos];
        heap->tree[ipos] = temp;

        ipos = ppos;
        ppos = Heap_parent(ipos);
    }

    heap->size++;

    return 0;
}



//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
int Heap_extract(Heap* heap, void** data)
{
    void* save;
    void* temp;

    int ipos;
    int lpos;
    int rpos;
    int mpos;

    // Do not allow extraction from an empty heap
    if (Heap_size(heap) == 0)
    {
        return -1;
    }

    // Extract the node at the top of the heap
    *data = heap->tree[0];

    // Adjust the storage used by the heap
    save = heap->tree[Heap_size(heap) - 1];

    if (Heap_size(heap) - 1 > 0)
    {
        temp = (void**)realloc(heap->tree, (Heap_size(heap) - 1) * sizeof(void*));
        if (temp == NULL)
        {
            return -1;
        }
        else
        {
            heap->tree = (void**) temp;
        }

        // Adjust the size of the heap to account for the extracted node
        heap->size--;
    }

    else
    {
        // Manage the heap when extracting the last node
        free(heap->tree);
        heap->tree = NULL;
        heap->size = 0;
        return 0;
    }

    // Copy the last node to the top
    heap->tree[0] = save;

    // Heapify the tree by pushing the contents of the new top downward
    ipos = 0;
    lpos = Heap_left(ipos);
    rpos = Heap_right(ipos);

    while (1)
    {
        // Select the child to swap with the current node
        lpos = Heap_left(ipos);
        rpos = Heap_right(ipos);

        if (lpos < Heap_size(heap) && heap->compare(heap->tree[lpos], heap->tree[ipos]) > 0)
        {
            mpos = lpos;
        }
        else
        {
            mpos = ipos;
        }

        if (rpos < Heap_size(heap) && heap->compare(heap->tree[rpos], heap->tree[mpos]) > 0)
        {
            mpos = rpos;
        }

        // When mpos is ipos, the heap property has been restored
        if (mpos == ipos)
        {
            break;
        }
        else
        {
            // Swap the contents of the current node and the selected child
            temp = heap->tree[mpos];
            heap->tree[mpos] = heap->tree[ipos];
            heap->tree[ipos] = temp;

            // Move down one level in the tree to continue heapifying
            ipos = mpos;
        }
    }

    return 0;
}
