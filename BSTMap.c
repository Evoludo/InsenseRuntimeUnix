/**
 * BSTMap.c
 * Implementation for Binary Search Tree based Map.
 *
 * Coded to contain void* pointers for keys and values in case we want to use 
 * the map for for a variety of objects.
 *
 * At present, the map is used to contain references to deserialiser functions
 * for constructed Insense types that are sent over the radio.
 * The key is a string code used to identify an Insense struct type and its 
 * corresponding deserialisation function. For example, a map node may contain 
 * the key "s#a:b#i:i" and the value pointing to the deserialisation function 
 * for the Insense struct_abintegerinteger.
 *
 * As map nodes are not shared among multiple maps the garbage collector 
 * only needs to destroy (clear) the map once a map object is no longer 
 * referenced. 
 * Consequently decided to use BASE_mem_alloc and free when constructing and 
 * destroying maps, the decRef function for the map simply calls the mapClear
 * function to clear the map and free memory used by map nodes.
 *
 * @author jonl 
 * 21.05.2008
 *
 */


#include <string.h>            // for strcmp used for string key comparisons
#include "BSTMap.h"            // for map
#include "DAL_mem.h"           // for memory management
#include "DAL_error.h"         // for DAL_error macro
#include "FunctionPair.h"     // for function pair payload in map

#ifndef DALSMALL
static char *file_name = "BSTMap"; // used by DAL_error macro
#endif
// "private" (static in C-sense) function declarations, not included in header.

static BSTMapNode_PNTR Construct_MapNode(char *key, void *value);
static BSTMapNode_PNTR *mapSearch(BSTMapNode_PNTR *root, 
				  char *searchKey);

// End of "private" function declarations

// (public) functions and constructors 
BSTMap_PNTR Construct_StringMap(){
 //   printf("Construct_StringMap\n");
  // allocate mem for map containing pointer to functions
  BSTMap_PNTR this = (BSTMap_PNTR) DAL_alloc(sizeof(BSTMap_Struct), false);
  if(this == 0){
    DAL_error(OUT_OF_MEMORY_ERROR);
    return 0;
  }
  // alloc sets to 0
  //this->root = NULL; // initially empty map
  return(this);
}

// insert key value pair into map
void mapPut(BSTMap_PNTR m, char *key, void *value) {
  BSTMapNode_PNTR newNode = Construct_MapNode(key, value);
  if(m == NULL){
    DAL_error(NULL_POINTER_ERROR);
    return;
  }
  if(newNode == NULL){
    DAL_error(OUT_OF_MEMORY_ERROR);
    return;
  }
  BSTMapNode_PNTR *foundNode = mapSearch(&m->root, newNode->key);
  *foundNode = newNode; // put new node into leaf node
}
 
// get hold of a value in the map by searching for key
void *mapGet(BSTMap_PNTR m, char *key){
  if(m == NULL){
    DAL_error(NULL_POINTER_ERROR);
    return NULL;
  }
  BSTMapNode_PNTR *foundNode = mapSearch(&m->root, key);
  if(*foundNode == NULL) {// map does not contain value for given key
    DAL_error(ELEMENT_NOT_FOUND_ERROR);
    return NULL;
  }
  return (*foundNode)->value; // otherwise return value found for given key
}



// "private" (static in C-sense) functions, not included in header.

static BSTMapNode_PNTR Construct_MapNode(char *key, void *value){
  // Not maintaining nodes using garbage collector, waste of space and 
  // time as nodes are not shared by multiple maps and you always know when 
  // a node has been created and removed from the map.
  // So use BASE_mem_alloc here and will use explicit BASE_mem_free
  // in freeMapNode function (declared above & defined at bottom of file)
  BSTMapNode_PNTR this =(BSTMapNode_PNTR) 
    BASE_mem_alloc(sizeof(BSTMapNode_Struct));
  if(this == 0){
    DAL_error(OUT_OF_MEMORY_ERROR);
    return 0;
  }
  this->key = key;
  this->value=NULL;
  DAL_assign(&this->value, value); // map payload is garbage collected
  this->left = NULL;
  this->right = NULL;
  return this;
}


static BSTMapNode_PNTR *mapSearch(BSTMapNode_PNTR *root, char *searchKey){
  BSTMapNode_PNTR *thisNode = root;
  while(*thisNode != NULL){    // if NULL we have found empty leaf node
    int comp_result = 
      strcmp(searchKey, (*thisNode)->key); // otherwise compare keys 
    if(comp_result < 0)       // search left  side of tree map
      thisNode = &((*thisNode)->left);
    else if(comp_result > 0)  // search right side of tree map
      thisNode = &((*thisNode)->right);
    else                      // comp_result is 0, i.e. have found node
      break;                  // break out of while loop
  }
  return thisNode; // return pointer to found node or to empty leaf where
}                   // findNode belongs if it was not found

