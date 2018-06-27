#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"

typedef struct node
{
	void* dataPtr;
	struct node* next;
} QUEUE_NODE;

typedef struct
{
	QUEUE_NODE* front;
	QUEUE_NODE* rear;
	int count;
} QUEUE;

QUEUE* createQueue();
QUEUE* destroyQueue(QUEUE* queue);

boolean enqueue(QUEUE* queue, void* itemPtr);
boolean dequeue(QUEUE* queue, void** itemPtr);
boolean queueFront(QUEUE* queue, void** itemPtr);
boolean queueRear(QUEUE* queue, void** itemPtr);
int queueCount(QUEUE* queue);

boolean emptyQueue(QUEUE* queue);
boolean fullQueue(QUEUE* queue);

QUEUE* createQueue()
{
	QUEUE* queue;

	queue = (QUEUE*)malloc(sizeof(QUEUE));
	if (queue)
	{
		queue -> front = NULL;
		queue -> rear = NULL;
		queue -> count = 0;
	}

	return queue;
}

QUEUE* destroyQueue(QUEUE* queue)
{
	QUEUE_NODE* deletePtr;

	if (queue)
	{
		while (queue -> front != NULL)
		{
			free(queue -> front -> dataPtr);
			deletePtr = queue -> front;
			queue -> front = queue -> front -> next;
			free(deletePtr);
		}
		free(queue);
	}

	return NULL;
}

boolean enqueue(QUEUE* queue, void* itemPtr)
{
	QUEUE_NODE* newPtr;

	if (!(newPtr = (QUEUE_NODE*)malloc(sizeof(QUEUE_NODE))))
		return false;

	newPtr -> dataPtr = itemPtr;
	newPtr -> next = NULL;

	if (queue -> count == 0)
		queue -> front = newPtr;
	else
		queue -> rear -> next = newPtr;

	(queue -> count)++;
	queue -> rear = newPtr;
	return true;
}

boolean dequeue(QUEUE* queue, void** itemPtr)
{
	QUEUE_NODE* deleteLoc;

	if (!queue -> count)
		return false;
	*itemPtr = queue -> front -> dataPtr;
	deleteLoc = queue -> front;
	if (queue -> count == 1)
		queue -> rear = queue -> front = NULL;
	else
		queue -> front = queue -> front -> next;
	(queue -> count)--;
	free(deleteLoc);

	return true;
}

boolean queueFront(QUEUE* queue, void** itemPtr)
{
	if (!queue -> count)
		return false;
	else
	{
		*itemPtr = queue -> front -> dataPtr;
		return true;
	}
}

boolean queueRear(QUEUE* queue, void** itemPtr)
{
	if (!queue -> count)
		return false;
	else
	{
		*itemPtr = queue -> rear -> dataPtr;
		return true;
	}
}

int queueCount(QUEUE* queue)
{
	return queue -> count;
}

boolean emptyQueue(QUEUE* queue)
{
	return (queue -> count == 0);
}

boolean fullQueue(QUEUE* queue)
{
	QUEUE_NODE* temp;

	temp = (QUEUE_NODE*)malloc(sizeof(*(queue -> rear)));
	if (temp)
	{
		free(temp);
		return true;
	}

	return false;
}