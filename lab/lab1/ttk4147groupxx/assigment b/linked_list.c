#include <stdlib.h>
#include <stdio.h>
#include "linked_list.h"

list_t list_create()
{

	int sizeOfList = sizeof(struct list);	
 	
	/* Prepare list container */	
	struct list* temp = 	 malloc(sizeOfList); 
	temp->length = 0;
	temp->head = NULL;
	temp->tail = NULL;

	return temp;
	
	

}

void list_delete(list_t list)
{
	//ensure data has been released
	while(list->length != 0){
		list_extract(list,0);
	}

	free(list);
}

void list_insert(list_t list, int index, int data)
{

	if ( index >= list->length ){
		list_append(list,data);
		return;
	}	

	if ( index == 0 ) {
		
		int sizeOfNode = sizeof(struct node);
		struct node* newNode = malloc(sizeOfNode);

		newNode->prev = NULL;
		newNode->next = list->head;
		newNode->data = data;	
		
		list->head->next->prev = newNode; // set previous heads next to new head

		list->head = newNode;			

		list->length++;
		return;
	}


	struct node * nodeIt;
	nodeIt = list->head;

	int i;

	for ( i = 0 ; i < index-1 ; i++ ){
		nodeIt = nodeIt->next;
	}	

	// create new node
	int sizeOfNode = sizeof(struct node);
	struct node* newNode = malloc(sizeOfNode);
	newNode->data = data;

	// point new node to existing nodes:
		// prev to current	
		// next to next
	newNode->prev = nodeIt;
	newNode->next = nodeIt->next;

	// point next node(prev) to new node
	nodeIt->next->prev = newNode;

	// point current node(next) to new node
	nodeIt->next = newNode;

	list->length++;

}

void list_append(list_t list, int data)
{

	int sizeOfNode = sizeof(struct node);

	if ( list->length == 0){
		struct node* firstNode = malloc(sizeOfNode);	
		firstNode->next = NULL;
		firstNode->prev = NULL;
		firstNode->data = data;

		list->head = firstNode;
		list->tail = firstNode;
	
		list->length++;
		return;
		
	}

	/* create new node */
	
	struct node* newNode = malloc(sizeOfNode);
	newNode->data = data;
	newNode->next = NULL;
	newNode->prev = list->tail;

	list->tail->next = newNode;	

	/* update end of list */
	list->tail = newNode;
	list->length++;
	
	

	

}

void list_print(list_t list)
{
	
	struct node * nodeIt;
	nodeIt = list->head;
	
	int i;

	for(i = 0; i < list->length; i++)
	{
		printf("%i\n",nodeIt->data);
		
		nodeIt = nodeIt->next;
	}

	

}

long list_sum(list_t list)
{

	struct node * nodeIt;
	nodeIt = list->head;

	int i;
	long sum = 0;
	
	for ( i = 0;  i < list->length ; i++ ){

		sum += nodeIt->data;
		nodeIt = nodeIt->next;		

	}

	return sum;
}

int list_get(list_t list, int index)
{

	struct node * nodeIt;
	nodeIt = list->head;

	int i;

	for ( i = 0 ; i < index ; i++ ){
		nodeIt = nodeIt->next;
	}

	return nodeIt->data;

}

int list_extract(list_t list, int index)
{

	if ( list->length == 1 ){
		
		struct node* oldNode = list->head;
		int oldData = oldNode->data;		
		list->head = NULL;
		list->tail = NULL;	
		free(oldNode);

		list->length = 0;
		return oldData;
		
		

	}

	
	if ( index == 0 ) {
		
		struct node* oldHead = list->head;
		struct node* newHead = list->head->next;
		int oldData 	     = oldHead->data;

		list->head = newHead;
		newHead->prev = NULL;
		
		free(oldHead);
		list->length--;
		return oldData;
	}
	
	if ( index == list->length-1 ){
		struct node* oldTail = list->tail;
		struct node* newTail = list->tail->prev;
		int oldData 	     = oldTail->data;

		list->tail = newTail;
		newTail->next = NULL;
		
		free(oldTail);

		list->length--;
		return oldData;
	}





	struct node * nodeIt;
	nodeIt = list->head;
	
	printf("Attempting to remove value on index %i\n",index);

	int i;
	
	for ( i = 0;  i < index; i++ ){
		nodeIt = nodeIt->next;		
	}

	struct node * prevNode = nodeIt->prev;
	struct node * nextNode = nodeIt->next;
	int deletedData = nodeIt->data;

	prevNode->next = nextNode;
	nextNode->prev = prevNode;

	list->length--;	

	free(nodeIt);
	
	return deletedData;
	
		
}
