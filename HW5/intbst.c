#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <assert.h> // assert
#include <time.h>	// time

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int data;
	struct node *left;
	struct node *right;
} NODE;

typedef struct
{
	NODE *root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *BST_Create(void)
{
	TREE *temp = (TREE *)malloc(sizeof(TREE));
	if (temp == NULL)
		return NULL;

	temp->root = NULL;

	return temp;
}

/* internal function (not mandatory)
*/
static void _destroy(NODE *root) {
	if (root == NULL)
		return;

	_destroy(root->left);
	_destroy(root->right);
	free(root);
}

/* Deletes all data in tree and recycles memory
*/
void BST_Destroy(TREE *pTree) {
	_destroy(pTree->root);

	free(pTree);
}

/* internal function (not mandatory)
*/
static void _insert(NODE *root, NODE *newPtr)
{
	if (newPtr->data < root->data)
	{
		if (root->left == NULL)
		{
			root->left = newPtr;
			return;
		}

		_insert(root->left, newPtr);
		return;
	}

	else
	{
		if (root->right == NULL)
		{
			root->right = newPtr;
			return;
		}

		_insert(root->right, newPtr);
		return;
	}
}

NODE *_makeNode(int data)
{
	NODE *temp = (NODE *)malloc(sizeof(NODE));

	if (temp == NULL)
		return NULL;

	temp->data = data;
	temp->left = temp->right = NULL;

	return temp;
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert(TREE *pTree, int data)
{
	NODE *newNode = _makeNode(data);

	if (newNode == NULL)
		return 0;

	if (pTree->root != NULL)
		_insert(pTree->root, newNode);

	else
		pTree->root = newNode;

	return 1;
}

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE *_delete(NODE *root, int dltKey, int *success)
{
	if (root == NULL)
	{
		*success = 0;
		return root;
	}

	if (dltKey < root->data)
		root->left = _delete(root->left, dltKey, success);

	else if (dltKey > root->data)
		root->right = _delete(root->right, dltKey, success);

	else
	{
		if (root->left == NULL)
		{
			NODE *delNode = root;
			root = root->right;
			free(delNode);
		}

		else if (root->right == NULL)
		{
			NODE *delNode = root;
			root = root->left;
			free(delNode);
		}

		else
		{
			NODE *temp = root->right;

			while (temp->left != NULL) {
				temp = temp->left;
			}

			root->data = temp->data;
			root->right = _delete(root->right, temp->data, success);
		}

		*success = 1;
	}

	return root;
}

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete(TREE *pTree, int dltKey)
{
	int success = 0;

	pTree->root = _delete(pTree->root, dltKey, &success);

	return success;
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve(NODE *root, int key) {
	if (root == NULL)
		return NULL;

	if (key < root->data)
		return _retrieve(root->left, key);

	else if (key > root->data)
		return _retrieve(root->right, key);

	else
		return root;
}

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int *BST_Retrieve(TREE *pTree, int key) {
	if(pTree) {
		NODE *temp = _retrieve(pTree->root, key);
		
		if (temp == NULL)
			return NULL;
		
		else
			return &(temp->data);
	}

	return NULL;
}

static void _traverse(NODE *root)
{
	if (root == NULL)
		return;

	_traverse(root->left);
	printf("%d ", root->data);
	_traverse(root->right);
}

/* prints tree using inorder traversal
*/
void BST_Traverse(TREE *pTree)
{
	_traverse(pTree->root);
}

/* internal traversal function
*/
static void _infix_print(NODE *root, int level)
{
	if (root == NULL)
		return;

	_infix_print(root->right, level + 1);
	for (int i = 0; i < level; i++)
		printf("\t");
	printf("%d\n", root->data);
	_infix_print(root->left, level + 1);
}

/* Print tree using inorder right-to-left traversal
*/
void printTree(TREE *pTree)
{
	_infix_print(pTree->root, 0);
}

/* 
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty(TREE *pTree)
{
	if (pTree == NULL || pTree->root == NULL)
		return 1;

	else
		return 0;
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	TREE *tree;
	int data;

	// creates a null tree
	tree = BST_Create();

	if (!tree)
	{
		printf("Cannot create a tree!\n");
		return 100;
	}

	fprintf(stdout, "How many numbers will you insert into a BST: ");

	int numbers;
	scanf("%d", &numbers);

	fprintf(stdout, "Inserting: ");

	srand(time(NULL));
	for (int i = 0; i < numbers; i++)
	{
		data = rand() % (numbers * 3) + 1; // random number (1 ~ numbers * 3)

		fprintf(stdout, "%d ", data);

		// insert funtion call
		int ret = BST_Insert(tree, data);
		if (!ret)
			break;
	}
	fprintf(stdout, "\n");

	// inorder traversal
	fprintf(stdout, "Inorder traversal: ");
	BST_Traverse(tree);
	fprintf(stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf(stdout, "Tree representation:\n");
	printTree(tree);

	while (1)
	{
		fprintf(stdout, "Input a number to delete: ");
		int num;
		int ret = scanf("%d", &num);
		if (ret != 1)
			break;

		ret = BST_Delete(tree, num);
		if (!ret)
		{
			fprintf(stdout, "%d not found\n", num);
			continue;
		}

		// print tree with right-to-left infix traversal
		fprintf(stdout, "Tree representation:\n");
		printTree(tree);

		if (BST_Empty(tree))
		{
			fprintf(stdout, "Empty tree!\n");
			break;
		}
	}

	BST_Destroy(tree);

	return 0;
}
