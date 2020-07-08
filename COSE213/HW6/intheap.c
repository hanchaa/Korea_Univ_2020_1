#include <stdio.h>
#include <stdlib.h> // malloc, rand
#include <time.h>	// time

#define MAX_ELEM 20

typedef struct
{
	int *heapArr;
	int last;
	int capacity;
} HEAP;

/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
*/
HEAP *heapCreate(int capacity)
{
	HEAP *temp = malloc(sizeof(HEAP));

	if (temp == NULL)
		return NULL;

	temp->heapArr = malloc(capacity * sizeof(int));

	if (temp->heapArr == NULL)
	{
		free(temp);
		return NULL;
	}

	temp->last = -1;
	temp->capacity = capacity;

	return temp;
}

/* Free memory for heap
*/
void heapDestroy(HEAP *heap)
{
	if (heap == NULL)
		return;

	free(heap->heapArr);
	free(heap);
}

/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp(HEAP *heap, int index)
{
	if (index == 0)
		return;

	if (heap->heapArr[index] > heap->heapArr[(index - 1) / 2])
	{
		int temp = heap->heapArr[index];
		heap->heapArr[index] = heap->heapArr[(index - 1) / 2];
		heap->heapArr[(index - 1) / 2] = temp;

		_reheapUp(heap, (index - 1) / 2);
	}
}

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heapInsert(HEAP *heap, int data)
{
	if (heap == NULL)
		return 0;

	heap->heapArr[++(heap->last)] = data;

	_reheapUp(heap, heap->last);

	return 1;
}

/* Reestablishes heap by moving data in root down to its correct location in the heap
*/
static void _reheapDown(HEAP *heap, int index)
{
	int left = -1, right = -1, subtree;

	if (index * 2 + 1 <= heap->last)
	{
		left = heap->heapArr[index * 2 + 1];

		if (index * 2 + 2 <= heap->last)
			right = heap->heapArr[index * 2 + 2];

		if (left > right)
			subtree = index * 2 + 1;

		else
			subtree = index * 2 + 2;

		if (heap->heapArr[index] < heap->heapArr[subtree])
		{
			int temp = heap->heapArr[index];
			heap->heapArr[index] = heap->heapArr[subtree];
			heap->heapArr[subtree] = temp;

			_reheapDown(heap, subtree);
		}
	}
}

/* Deletes root of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heapDelete(HEAP *heap, int *data)
{
	if (heap == NULL)
		return 0;

	*data = heap->heapArr[0];

	int temp = heap->heapArr[0];
	heap->heapArr[0] = heap->heapArr[heap->last];
	heap->heapArr[heap->last] = temp;

	(heap->last)--;

	_reheapDown(heap, 0);

	return 1;
}

/* Print heap array */
void heapPrint(HEAP *heap)
{
	if (heap == NULL)
		return;

	for (int i = 0; i <= heap->last; i++)
		printf("%6d", heap->heapArr[i]);

	printf("\n");

	if (heap->last == heap->capacity - 1)
		printf("\n");
}

int main(void)
{
	HEAP *heap;
	int data;
	int i;

	heap = heapCreate(MAX_ELEM);

	srand(time(NULL));

	for (i = 0; i < MAX_ELEM; i++)
	{
		data = rand() % MAX_ELEM * 3 + 1; // 1 ~ MAX_ELEM*3 random number

		fprintf(stdout, "Inserting %d: ", data);

		// insert function call
		heapInsert(heap, data);

		heapPrint(heap);
	}

	while (heap->last >= 0)
	{
		// delete function call
		heapDelete(heap, &data);

		printf("Deleting %d: ", data);

		heapPrint(heap);
	}

	heapDestroy(heap);

	return 0;
}
