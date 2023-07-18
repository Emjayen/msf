/*
 * lnklist.cpp
 *
 */
#include "..\inc\lnklist.h"







/*
 * list_append
 *    Add an element to the end of a list
 *
 */
void pce_fastcall list_append(list* pList, list_node* pNewNode)
{
	pNewNode->next = NULL;
	pNewNode->prev = NULL;

	if(!pList->head)
	{
		pList->head = pNewNode;
		pList->tail = pNewNode;
	}

	//else
	//	list_insert_before(pList, pList->head, pNewNode);

	else
	{
		pNewNode->prev = pList->tail;
		pList->tail->next = pNewNode;
		pList->tail = pNewNode;
	}

	pList->count++;
}


/*
 * list_insert_after
 *   Insert an element after a given node.
 *
 */
void pce_fastcall list_insert_after(list* pList, list_node* pNode, list_node* pNewNode)
{
	pNewNode->prev = pNode;

	if(!pNode->next)
	{
		pNewNode->next = NULL;
		pList->tail = pNewNode;
	}
	
	else
	{
		pNewNode->next = pNode->next;
		pNode->next->prev = pNewNode;
	}

	pNode->next = pNewNode;
	pList->count++;
}


/*
 * list_insert_before
 *   Insert an element before a given node.
 *
 */
void pce_fastcall list_insert_before(list* pList, list_node* pNode, list_node* pNewNode)
{
	pNewNode->next = pNode;
	
	if(!pNode->prev)
	{
		pNewNode->prev = NULL;
		pList->head = pNewNode;
	}

	else
	{
		pNewNode->prev = pNode->prev;
		pNode->next->next = pNewNode;
	}

	pNode->prev = pNewNode;
	pList->count++;
}


/*
 * list_remove
 *   Remove an element from a list
 *
 */
void pce_fastcall list_remove(list* pList, list_node* pNode)
{
	if(!pNode->prev)
		pList->head = pNode->next;

	else
		pNode->prev->next = pNode->next;

	if(!pNode->next)
		pList->tail = pNode->prev;

	else
		pNode->next->prev = pNode->prev;

	pNode->next = NULL;
	pNode->prev = NULL;

	pList->count--;
}