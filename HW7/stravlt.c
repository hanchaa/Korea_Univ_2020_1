#define SHOW_STEP 0
#define BALANCING 1 // used in _insert function

#include <stdlib.h> // malloc, rand
#include <stdio.h>
#include <time.h>	// time
#include <string.h> //strcmp, strdup

#define max(x, y) (((x) > (y)) ? (x) : (y))

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
	char *data;
	struct node *left;
	struct node *right;
	int height;
} NODE;

typedef struct
{
	NODE *root;
	int count; // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE *AVL_Create(void)
{
	AVL_TREE *temp = (AVL_TREE *)malloc(sizeof(AVL_TREE));

	if (temp == NULL)
		return NULL;

	temp->root = NULL;
	temp->count = 0;

	return temp;
}

static void _destroy(NODE *root)
{
	if (root == NULL)
		return;

	_destroy(root->left);
	_destroy(root->right);

	free(root->data);
	free(root);
}

/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy(AVL_TREE *pTree)
{
	_destroy(pTree->root);

	free(pTree);
}

static NODE *_makeNode(char *data)
{
	NODE *temp = (NODE *)malloc(sizeof(NODE));
	if (temp == NULL)
		return NULL;

	temp->data = strdup(data);
	if (temp->data == NULL)
	{
		free(temp);
		return NULL;
	}

	temp->height = 1;
	temp->left = temp->right = NULL;

	return temp;
}

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight(NODE *root)
{
	if (root == NULL)
		return 0;

	return root->height;
}

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight(NODE *root)
{
	NODE *temp = root;
	root = root->left;
	temp->left = root->right;
	root->right = temp;

	root->right->height = max(getHeight(root->right->left), getHeight(root->right->right)) + 1;
	root->height = max(getHeight(root->left), getHeight(root->right)) + 1;

	return root;
}

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE *rotateLeft(NODE *root)
{
	NODE *temp = root;
	root = root->right;
	temp->right = root->left;
	root->left = temp;

	root->left->height = max(getHeight(root->left->left), getHeight(root->left->right)) + 1;
	root->height = max(getHeight(root->left), getHeight(root->right)) + 1;

	return root;
}

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert(NODE *root, NODE *newPtr)
{
	if (root == NULL)
		return newPtr;

	int res = strcmp(newPtr->data, root->data);

	if (res < 0)
		root->left = _insert(root->left, newPtr);

	else
		root->right = _insert(root->right, newPtr);

	root->height = max(getHeight(root->left), getHeight(root->right)) + 1;

#if BALANCING
	int lHeight = getHeight(root->left);
	int rHeight = getHeight(root->right);

	if (lHeight - rHeight > 1)
	{
		int subLHeight = getHeight(root->left->left);
		int subRHeight = getHeight(root->left->right);

		if (max(subRHeight, subLHeight) == subRHeight)
			root->left = rotateLeft(root->left);

		root = rotateRight(root);
	}

	else if (rHeight - lHeight > 1)
	{
		int subLHeight = getHeight(root->right->left);
		int subRHeight = getHeight(root->right->right);

		if (max(subLHeight, subRHeight) == subLHeight)
			root->right = rotateRight(root->right);

		root = rotateLeft(root);
	}
#endif

	return root;
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert(AVL_TREE *pTree, char *data)
{
	NODE *newNode = _makeNode(data);

	if (newNode == NULL)
		return 0;

	pTree->root = _insert(pTree->root, newNode);
	(pTree->count)++;

	return 1;
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve(NODE *root, char *key)
{
	if (root == NULL)
		return NULL;

	int res = strcmp(key, root->data);

	if (res == 0)
		return root;

	else if (res > 0)
		return _retrieve(root->right, key);

	else
		return _retrieve(root->left, key);
}

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
char *AVL_Retrieve(AVL_TREE *pTree, char *key)
{
	NODE *res = _retrieve(pTree->root, key);

	if (res == NULL)
		return NULL;

	return res->data;
}

static void _traverse(NODE *root)
{
	if (root == NULL)
		return;

	_traverse(root->left);
	printf("%s ", root->data);
	_traverse(root->right);
}

/* Prints tree using inorder traversal
*/
void AVL_Traverse(AVL_TREE *pTree)
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
	printf("%s\n", root->data);
	_infix_print(root->left, level + 1);
}

/* Prints tree using inorder right-to-left traversal
*/
void printTree(AVL_TREE *pTree)
{
	_infix_print(pTree->root, 0);
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	AVL_TREE *tree;
	char str[1024];

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s FILE\n", argv[0]);
		return 0;
	}

	// creates a null tree
	tree = AVL_Create();

	if (!tree)
	{
		fprintf(stderr, "Cannot create tree!\n");
		return 100;
	}

	FILE *fp = fopen(argv[1], "rt");
	if (fp == NULL)
	{
		fprintf(stderr, "Cannot open file! [%s]\n", argv[1]);
		return 200;
	}

	while (fscanf(fp, "%s", str) != EOF)
	{

#if SHOW_STEP
		fprintf(stdout, "Insert %s>\n", str);
#endif
		// insert function call
		AVL_Insert(tree, str);

#if SHOW_STEP
		fprintf(stdout, "Tree representation:\n");
		printTree(tree);
#endif
	}

	fclose(fp);

#if SHOW_STEP
	fprintf(stdout, "\n");

	// inorder traversal
	fprintf(stdout, "Inorder traversal: ");
	AVL_Traverse(tree);
	fprintf(stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf(stdout, "Tree representation:\n");
	printTree(tree);
#endif
	fprintf(stdout, "Height of tree: %d\n", tree->root->height);
	fprintf(stdout, "# of nodes: %d\n", tree->count);

	// retrieval
	char *key;
	fprintf(stdout, "Query: ");
	while (fscanf(stdin, "%s", str) != EOF)
	{
		key = AVL_Retrieve(tree, str);

		if (key)
			fprintf(stdout, "%s found!\n", key);
		else
			fprintf(stdout, "%s NOT found!\n", str);

		fprintf(stdout, "Query: ");
	}

	// destroy tree
	AVL_Destroy(tree);

	return 0;
}
