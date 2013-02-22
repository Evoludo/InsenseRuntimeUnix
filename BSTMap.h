/**
 * BSTMap.h
 * Header file for Binary Search Tree based Map implementation.
 *
 * @author jonl
 *
*/



#ifndef BSTMAP_H_
#define BSTMAP_H_

#include "Bool.h"

typedef struct bstmapnode BSTMapNode_Struct, *BSTMapNode_PNTR;
struct bstmapnode {
  char *key;
  void *value;
  BSTMapNode_PNTR left, right;
};

typedef struct bstmap BSTMap_Struct, *BSTMap_PNTR;
struct bstmap {
  //void (*decRef)(BSTMap_PNTR pntr); //pointer to decRef for garbage collector
  BSTMapNode_PNTR root;   // the root node in the BSTMap, NULL if map is empty
};


// constructor
extern BSTMap_PNTR Construct_StringMap();

// functions to insert into, get and remove elements from a map ob
// as well as clear a map
extern void mapPut(BSTMap_PNTR map, char *key, void *element);
extern void *mapGet(BSTMap_PNTR map, char *key);

#endif /* DALRADIO */
