// LinkedList.c

// BetterTelnet
// copyright 1997, 1998, 1999 Rolf Braun

// This is free software under the GNU General Public License (GPL). See the file COPYING
// which comes with the source code and documentation distributions for details.

// based on NCSA Telnet 2.7b5

#include "LinkedList.proto.h"

short reSortElement(LinkedListNode **theHead, LinkedListNode *theChangedElement)
{
	LinkedListNode *leader, *copyOfNew;
	short index = 1;
	copyOfNew = (LinkedListNode *) myNewPtrCritical(sizeof(LinkedListNode));
	copyOfNew->name = NewString(*theChangedElement->name);
	copyOfNew->next = NULL;
	deleteItem(theHead, *theChangedElement->name);
	insertInSortedList(theHead, copyOfNew);
	
	//now its in there; return where it is
	
	leader = *theHead;
	while ((leader->next != NULL)&&
		(!(EqualString(*leader->name,*copyOfNew->name,TRUE,FALSE))))
	{
		leader = leader->next;
		index++;
	}
	return index - 1;
}

void deleteList(LinkedListNode **theHead)
{
	LinkedListNode *current,*nextNode;
	current = *theHead;
	if (current == NULL)
		return;
	nextNode = current->next;
	while(current != NULL)
	{
		DisposeHandle((Handle)current->name);
		DisposePtr((Ptr)current);
		current = nextNode;
										// RJZ, 12/10/97.  Avoid dereferencing
										// a NULL pointer.
		if (current != NULL)
			nextNode = nextNode->next;
	}
	*theHead = NULL;
}
void insertInSortedList(LinkedListNode **head, LinkedListNode *newItem)
{
	LinkedListNode  *leader, *trailer;
	Boolean done = FALSE;
	short newOneIsBigger;

	if (*head == NULL)//this is first element in list
	{
		*head = newItem;
		return;
	}
	trailer = *head;
	leader = (*head)->next;
	if (leader == NULL)
	{
		(*head)->next = newItem;
		newItem->next = NULL;
		return;
	}
	
	while (!done)
	{
		newOneIsBigger = RelString(*(newItem->name),*(leader->name),TRUE,FALSE);
		if (newOneIsBigger == 1) 
		{
			if (leader->next == NULL)//end of list
			{
				leader->next = newItem;
				newItem->next = NULL;
				done = TRUE;
			}
			else
			{
				trailer = leader;
				leader = leader->next;
			}
		}
		else
		{
			trailer->next = newItem;
			newItem->next = leader;
			done = TRUE;
		}
	}
}	
//returns position
short createNodeAndSort(LinkedListNode **head, Str255 theName)
{
	short index=1;
	LinkedListNode *leader = *head;
	LinkedListNode *newNode = (LinkedListNode *)myNewPtrCritical(sizeof(LinkedListNode));
	newNode->name = NewString(theName);
	newNode->next = NULL;
	if (*head == NULL)
	{
		*head = newNode;
		return 1;
	}
	else
	{
		insertInSortedList(head, newNode);
		
		while ((leader->next != NULL)&&
			(!(EqualString(*leader->name,theName,TRUE,FALSE))))
		{
			leader = leader->next;
			index++;
		}
		return (index - 1);
	}
}
	
LinkedListNode *createSortedList(ResType ConfigResourceType,short numberofitems,ConstStr255Param placeThisFirst)
{
	short 		index, resID;
 	Handle		ItemResource;
	Str255		ItemName;
	ResType		restype;
	LinkedListNode *newNode, *theHead = NULL;
	
	if (placeThisFirst) {
		ItemResource = Get1NamedResource(ConfigResourceType,placeThisFirst);
		if ( ItemResource ) {
			theHead = (LinkedListNode *) myNewPtrCritical(sizeof(LinkedListNode));
			theHead->name = NewString(placeThisFirst); //set the head of our list
			theHead->next = NULL;
			ReleaseResource(ItemResource);
		}
	}
	for (index = 1; index <= numberofitems; index++) {
		ItemResource = Get1IndResource(ConfigResourceType, index);
		if ( ItemResource ) {
			GetResInfo(ItemResource, &resID, &restype, ItemName);
			if ( !placeThisFirst || !EqualString(placeThisFirst,ItemName,1,0) ) {
				newNode = (LinkedListNode *) myNewPtrCritical(sizeof(LinkedListNode));
				newNode->name = NewString(ItemName);
				if ( theHead == NULL ) {
					theHead = newNode;
					theHead->next = NULL;
				} else {
					insertInSortedList(&theHead, newNode);
				}
				ReleaseResource(ItemResource);
			}
		}
	}
	return theHead;
}

LinkedListNode *createSortedList2(ResType ConfigResourceType,short numberofitems,ConstStr255Param placeThisFirst)
{
	short 		index, resID;
 	Handle		ItemResource;
	Str255		ItemName;
	ResType		restype;
	LinkedListNode *newNode, *theHead = NULL;
	
	if (placeThisFirst) {
		ItemResource = GetNamedResource(ConfigResourceType,placeThisFirst);
		if ( ItemResource ) {
			theHead = (LinkedListNode *) myNewPtrCritical(sizeof(LinkedListNode));
			theHead->name = NewString(placeThisFirst); //set the head of our list
			theHead->next = NULL;
			ReleaseResource(ItemResource);
		}
	}
	for (index = 1; index <= numberofitems; index++) {
		ItemResource = GetIndResource(ConfigResourceType, index);
		if ( ItemResource ) {
			GetResInfo(ItemResource, &resID, &restype, (StringPtr)&ItemName);
			if ( !placeThisFirst || !EqualString(placeThisFirst,ItemName,1,0) ) {
				newNode = (LinkedListNode *) myNewPtrCritical(sizeof(LinkedListNode));
				newNode->name = NewString(ItemName);
				if ( theHead == NULL ) {
					theHead = newNode;
					theHead->next = NULL;
				} else {
					insertInSortedList(&theHead, newNode);
				}
				ReleaseResource(ItemResource);
			}
		}
	}
	return theHead;
}

void deleteItem(LinkedListNode **theHead,Str255 ItemName)
{
	LinkedListNode *leader = *theHead, *trailer = *theHead;
	while(!(EqualString(ItemName,*leader->name,TRUE,FALSE))&&(leader->next != NULL))
	{
		trailer = leader;
		leader = leader->next;
	}
	if (leader == *theHead)
	{
		*theHead = (*theHead)->next;
		DisposeHandle((Handle)leader->name);
		DisposePtr((Ptr)leader);
	}
	else
	{
		trailer->next = leader->next;
		DisposeHandle((Handle)leader->name);
		DisposePtr((Ptr)leader);
	}
}

LinkedListNode *findNode(LinkedListNode *theHead,Str255 ItemName)
{
	LinkedListNode *leader = theHead;
	while ((leader->next != NULL)&&(!(EqualString(ItemName,*leader->name,TRUE,FALSE))))
		leader = leader->next;
	
	if (!(EqualString(ItemName,*leader->name,TRUE,FALSE)))
		return NULL;
	else
		return(leader);
}

LinkedListNode *findNodeNC(LinkedListNode *theHead,Str255 ItemName)
{
	LinkedListNode *leader = theHead;
	while ((leader->next != NULL)&&(!(EqualString(ItemName,*leader->name,FALSE,FALSE))))
		leader = leader->next;
	
	if (!(EqualString(ItemName,*leader->name,FALSE,FALSE)))
		return NULL;
	else
		return(leader);
}

short findClosestNodeIndex(LinkedListNode *theHead,Str255 ItemName)
{
	short index=1;
	LinkedListNode *leader = theHead, *trailer = theHead;
	while ((leader->next != NULL) &&
	(RelString(*leader->name,ItemName,TRUE,FALSE) == -1))
	{
		trailer = leader;
		leader = leader->next;
		index++;
	}
	return index-1;
}
			
short findNodeIndex(LinkedListNode *theHead,Str255 ItemName)
{
	short index = 1;
	LinkedListNode *leader = theHead;
	while ((leader->next != NULL)&&
		(!(EqualString(*leader->name,ItemName,TRUE,FALSE))))
	{
		leader = leader->next;
		index++;
	}
	
	return (index - 1);
}

void addListToMenu(MenuHandle theMenu, LinkedListNode *theHead, short itemIndex)
{
	LinkedListNode *traverser = theHead;
//	short itemIndex = 1;

	while(traverser != NULL)
	{
		HLock((Handle)traverser->name);
		AppendMenu(theMenu,"\poops"); //these two avoid diacritcal interpertation of '<'
		SetMenuItemText(theMenu,itemIndex,*traverser->name); 
		HUnlock((Handle)traverser->name);

		traverser = traverser->next;
		itemIndex++;
	}
}

void getAUniqueName(LinkedListNode *theHead, Str255 theName)
{
	while(findNodeNC(theHead,theName))
	{
		if ((theName[theName[0]] > '9')||
			(theName[theName[0]] < '0')) //add a number
		{
			theName[++theName[0]] = ' ';
			theName[++theName[0]] = '1';
		}
		else if (theName[theName[0]] == '9') //add another digit
		{
			theName[theName[0]] = '-';
			theName[++theName[0]] = '1';
		}	
		else
			theName[theName[0]]++; //incremement the number		
	}
}