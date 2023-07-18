/*
 * lnklist.h
 *
 */
#ifndef PCE_LNKLIST_H
#define PCE_LNKLIST_H
#include "..\pce.h"




struct list_node
{
	list_node* next;
	list_node* prev;
};


struct list
{
	list_node* head;
	list_node* tail;
	uint count;
};






/*
 * list_append
 *    Add an element to the end of a list
 *
 */
void pce_fastcall list_append(list* pList, list_node* pNewNode);

/*
 * list_remove
 *   Remove an element from a list
 *
 */
void pce_fastcall list_remove(list* pList, list_node* pNewNode);

/*
 * list_insert_after
 *   Insert an element after a given node.
 *
 */
void pce_fastcall list_insert_after(list* pList, list_node* pPrevNode, list_node* pNode);

/*
 * list_insert_before
 *   Insert an element before a given node.
 *
 */
void pce_fastcall list_insert_before(list* pList, list_node* pPrevNode, list_node* pNode);




#endif