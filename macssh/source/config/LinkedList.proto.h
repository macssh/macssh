//LinkedList.proto.h

#include "LinkedList.h"

LinkedListNode *createSortedList(ResType ConfigResourceType,short numberofitems,ConstStr255Param placeThisFirst);
LinkedListNode *createSortedList2(ResType ConfigResourceType,short numberofitems,ConstStr255Param placeThisFirst);
void deleteItem(LinkedListNode **theHead,Str255 ItemName);
LinkedListNode *findNode(LinkedListNode *theHead,Str255 ItemName);
LinkedListNode *findNodeNC(LinkedListNode *theHead,Str255 ItemName);
short reSortElement(LinkedListNode **theHead, LinkedListNode *theChangedElement);
void insertInSortedList(LinkedListNode **head, LinkedListNode *newItem);
void deleteList(LinkedListNode **theHead);
short createNodeAndSort(LinkedListNode **head, Str255 theName);
short findNodeIndex(LinkedListNode *theHead,Str255 ItemName);
short findClosestNodeIndex(LinkedListNode *theHead,Str255 ItemName);
void addListToMenu(MenuHandle, LinkedListNode*, short);
void getAUniqueName(LinkedListNode *theHead, Str255 theName);
