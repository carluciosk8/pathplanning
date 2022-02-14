/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __DLIST_H__
#define __DLIST_H__

#include <stdlib.h>


typedef struct DlistItem
{
    void*              data;
    struct DlistItem*  prev;
    struct DlistItem*  next;
}
DlistItem;


typedef struct
{
    int         size;
    DlistItem*  head;
    DlistItem*  tail;
    int   (*match)   (const void *key1, const void *key2);
    void  (*destroy) (void *data);
}
Dlist;


void DList_init       (Dlist* list, void (*destroy)(void* data));
void DList_destroy    (Dlist* list);
int  DList_insertNext (Dlist* list, DlistItem* item, const void* data);
int  DList_insertPrev (Dlist* list, DlistItem* item, const void* data);
int  DList_remove     (Dlist* list, DlistItem* item, void** data);


#define DList_size(list)       ((list)->size)
#define DList_head(list)       ((list)->head)
#define DList_tail(list)       ((list)->tail)
#define DList_isHead(element)  ((element)->prev == NULL ? 1 : 0)
#define DList_isTail(element)  ((element)->next == NULL ? 1 : 0)
#define DList_data(element)    ((element)->data)
#define DList_next(element)    ((element)->next)
#define DList_prev(element)    ((element)->prev)


#endif
