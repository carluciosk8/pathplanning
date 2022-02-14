/***************************************************************************
    Original code written in: Oct 2001
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __PATH_H_
#define __PATH_H_


#include "dlist.h"
#include "node.h"


typedef Dlist Path;

#define Path_init(path)   DList_init(path, Node_destroy)

void Path_create(Path* path, Node* node);


#endif
