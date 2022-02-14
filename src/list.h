/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __LIST_H__
#define __LIST_H__


typedef struct ListItem
{
    void*             data;
    struct ListItem*  next;
}
ListItem;


typedef struct List
{
    ListItem*  head;
    ListItem*  tail;
    int  size;
    int  (*match)   (const void* key1, const void* key2);
    void (*destroy) (void* data);
}
List;


void List_init       (List* List, void (*destroy)(void* data));
void List_destroy    (List* List);
int  List_insertNext (List* List, ListItem* element, const void* data);
int  List_removeNext (List* List, ListItem* element, void** data);

#define List_size(list)             ((list)->size)
#define List_head(list)             ((list)->head)
#define List_tail(list)             ((list)->tail)
#define List_isHead(list, element)  ((element) == (list)->head ? 1 : 0)
#define List_isTail(element)        ((element)->next == NULL ? 1 : 0)
#define List_data(element)          ((element)->data)
#define List_next(element)          ((element)->next)


#endif
