#include <stdlib.h> // malloc, atoi
#include <stdio.h>
#include <ctype.h>	// isdigit
#include <assert.h> // assert

#define MAX_STACK_SIZE 50

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	char data;
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
TREE *createTree(void)
{
	TREE *temp = (TREE *)malloc(sizeof(TREE));

	if (temp == NULL)
		return NULL;

	temp->root = NULL;

	return temp;
}

/* Deletes all data in tree and recycles memory
*/
void destroyTree(TREE *pTree);

static void _destroy(NODE *root)
{
	if (root == NULL)
		return;

	_destroy(root->left);
	_destroy(root->right);
	free(root);
}

/*  Allocates dynamic memory for a node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
static NODE *_makeNode(char ch)
{
	NODE *temp = (NODE *)malloc(sizeof(NODE));

	if (temp == NULL)
		return NULL;

	temp->data = ch;
	temp->right = temp->left = NULL;
	return temp;
}

// invalid expression일 경우 스택에 있는 모든 노드 제거
int invalid_expression(NODE **stack, int top)
{
	while (top >= 0)
	{
		_destroy(*(stack + top));
		*(stack + top) = NULL;
		top--;
	}

	return 0;
}

/* converts postfix expression to binary tree
	return	1 success
			0 invalid postfix expression
*/
int postfix2tree(char *expr, TREE *pTree)
{
	NODE *stack[MAX_STACK_SIZE] = {0};
	int top = -1, i = 0;

	while (*(expr + i) != '\0')
	{
		if (!isdigit(*(expr + i)))
		{
			if (*(expr + i) != '+' && *(expr + i) != '-' && *(expr + i) != '*' && *(expr + i) != '/')
				return invalid_expression(stack, top);

			if (top < 1)
				return invalid_expression(stack, top);

			NODE *newNode = _makeNode(*(expr + i));
			if (newNode == NULL)
				return invalid_expression(stack, top); // overflow 대신 invalid expression 출력

			newNode->right = stack[top];
			stack[top--] = NULL;
			newNode->left = stack[top];
			stack[top--] = NULL;

			stack[++top] = newNode;
		}

		else
		{
			NODE *newNode = _makeNode(*(expr + i));
			if (newNode == NULL)
				return invalid_expression(stack, top); // overflow 대신 invalid expression 출력

			stack[++top] = newNode;
		}

		i++;
	}

	if (top > 0)
		return invalid_expression(stack, top);

	pTree->root = stack[top];

	return 1;
}

/* Print node in tree using inorder traversal
*/
void traverseTree(TREE *pTree);

/* internal traversal function
	an implementation of ALGORITHM 6-6
*/
static void _traverse(NODE *root)
{
	if (root == NULL)
		return;

	if (isdigit(root->data))
		printf("%c", root->data);

	else
	{
		printf("(");
		_traverse(root->left);
		printf("%c", root->data);
		_traverse(root->right);
		printf(")");
	}
}

/* Print tree using inorder right-to-left traversal
*/
void printTree(TREE *pTree);

/* internal traversal function
*/
static void _infix_print(NODE *root, int level)
{
	if (root == NULL)
		return;

	_infix_print(root->right, level + 1);
	for (int i = 0; i < level; i++)
		printf("\t");
	printf("%c\n", root->data);
	_infix_print(root->left, level + 1);
}

/* evaluate postfix expression
	return	value of expression
*/
float evalPostfix(char *expr)
{
	float stack[MAX_STACK_SIZE] = {0};
	int i = 0, top = -1;

	while (*(expr + i) != '\0')
	{
		float left, right;

		if (!isdigit(*(expr + i)))
		{
			right = stack[top];
			stack[top--] = 0;
			left = stack[top];
			stack[top--] = 0;

			switch (*(expr + i))
			{
			case '+':
				stack[++top] = left + right;
				break;

			case '-':
				stack[++top] = left - right;
				break;

			case '*':
				stack[++top] = left * right;
				break;

			case '/':
				stack[++top] = left / right;
				break;
			}
		}

		else
			stack[++top] = *(expr + i) - '0';

		i++;
	}

	return stack[top];
}

////////////////////////////////////////////////////////////////////////////////
void destroyTree(TREE *pTree)
{
	if (pTree)
	{
		_destroy(pTree->root);
	}

	free(pTree);
}

////////////////////////////////////////////////////////////////////////////////
void printTree(TREE *pTree)
{
	_infix_print(pTree->root, 0);

	return;
}

////////////////////////////////////////////////////////////////////////////////
void traverseTree(TREE *pTree)
{
	_traverse(pTree->root);

	return;
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	TREE *tree;
	char expr[1024];

	fprintf(stdout, "\nInput an expression (postfix): ");

	while (fscanf(stdin, "%s", expr) == 1)
	{
		// creates a null tree
		tree = createTree();

		if (!tree)
		{
			printf("Cannot create tree\n");
			return 100;
		}

		// postfix expression -> expression tree
		int ret = postfix2tree(expr, tree);
		if (!ret)
		{
			fprintf(stdout, "invalid expression!\n");
			destroyTree(tree);
			continue;
		}

		// expression tree -> infix expression
		fprintf(stdout, "\nInfix expression : ");
		traverseTree(tree);

		// print tree with right-to-left infix traversal
		fprintf(stdout, "\n\nTree representation:\n");
		printTree(tree);

		// evaluate postfix expression
		float val = evalPostfix(expr);
		fprintf(stdout, "\nValue = %f\n", val);

		// destroy tree
		destroyTree(tree);

		fprintf(stdout, "\nInput an expression (postfix): ");
	}
	return 0;
}
