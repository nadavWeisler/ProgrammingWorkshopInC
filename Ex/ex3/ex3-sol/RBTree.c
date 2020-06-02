#include <stdio.h>
#include <stdlib.h>
#include "RBTree.h"
#include "utilities/RBUtilities.h"
#define TRUE 1
#define FALSE 0

#define BALANCE_POINT 0
#define RIGHT 1
#define LEFT -1
#define ME 0
#define NONE -2

/**
 *
 * @param tree
 * @param data
 * @param N
 */
void InsertNodeToTree(RBTree* tree, const void* data, Node* N);

/**
 *
 * @param tree
 * @param C
 * @param P
 * @param S
 */
void DbCase(RBTree* tree, Node* C, Node* P, Node* S);

/**
 *
 * @param compFunc
 * @param root
 * @param data
 * @return
 */
int getSide(Node* node, const void* data, CompareFunc compFunc)
{
	if (node == NULL)
	{
		return NONE;
	}
	int val = compFunc(data, node->data);
	if (val > BALANCE_POINT)
	{
		return RIGHT;
	}
	else if (val < BALANCE_POINT)
	{
		return LEFT;
	}
	else
	{
		return ME;
	}
}

/**
 *
 * @param node
 * @param compareFunc
 * @return
 */
Node* getSister(Node* node, CompareFunc compareFunc)
{
	Node* P = node->parent;
	int nodeSide = getSide(P, node->data, compareFunc);
	switch (nodeSide)
	{
		case LEFT:
			return P->right;
		case RIGHT:
			return P->left;
		default:
			return NULL;
	}
}

/**
 *
 * @param tree
 * @param node
 */
void leftRotation(RBTree* tree, Node* node)
{
	Node* rNode = node->right;
	node->right = rNode->left;

	if (node->right != NULL)
	{
		node->right->parent = node;
	}

	rNode->parent = node->parent;

	if (node->parent == NULL)
	{
		tree->root = rNode;
	}
	else if (node == node->parent->left)
	{
		node->parent->left = rNode;
	}
	else
	{
		node->parent->right = rNode;
	}

	rNode->left = node;
	node->parent = rNode;
}

/**
 *
 * @param tree
 * @param node
 */
void rightRotation(RBTree* tree, Node* node)
{
	Node* lNode = node->left;
	node->left = lNode->right;

	if (node->left != NULL)
	{
		node->left->parent = node;
	}

	lNode->parent = node->parent;

	if (node->parent == NULL)
	{
		tree->root = lNode;
	}
	else if (node == node->parent->left)
	{
		node->parent->left = lNode;
	}
	else
	{
		node->parent->right = lNode;
	}

	lNode->right = node;
	node->parent = lNode;
}

/**
 *
 * @param node
 * @return
 */
int isBlack(Node* node)
{
	return node == NULL || node->color == BLACK;
}

/**
 * To DO
 * @param tree
 * @return
 */
void paintTreeAfterInsert(RBTree* tree, Node* N)
{
	Node* P = N->parent;
	if (isBlack(P) == TRUE)
	{
		N->color = RED;
	}
	else
	{
		Node* U = getSister(P, tree->compFunc);
		Node* G = P->parent;
		if (isBlack(U) == FALSE)
		{
			P->color = BLACK;
			U->color = BLACK;
			G->color = RED;
			paintTreeAfterInsert(tree, P->parent);
		}
		else
		{
			if (P == G->left && N == P->right)
			{
				leftRotation(tree, P);
			}
			else if (P == G->right && N == P->left)
			{
				rightRotation(tree, P);
			}

			if (P == G->right && N == P->right)
			{
				leftRotation(tree, G);
			}
			else if (P == G->left && N == P->left)
			{
				rightRotation(tree, G);
			}

			G->color = RED;
			P->color = BLACK;
		}
	}
	tree->root->color = BLACK;
}

/**
 *
 * @param tree
 * @param data
 * @return
 */
Node* getNodeByData(RBTree* tree, void* data)
{
	Node* currentNode = tree->root;
	while (currentNode != NULL)
	{
		int side = getSide(currentNode, data, tree->compFunc);
		if (side == LEFT)
		{
			currentNode = currentNode->left;
		}
		else if (side == RIGHT)
		{
			currentNode = currentNode->right;
		}
		else
		{
			break;
		}
	}
	return currentNode;
}

/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
RBTree* newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
	RBTree* newTree = (RBTree*)malloc(sizeof(RBTree));
	newTree->root = NULL;
	newTree->size = 0;
	newTree->compFunc = compFunc;
	newTree->freeFunc = freeFunc;
	return newTree;
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int insertToRBTree(RBTree* tree, void* data)
{
	if (RBTreeContains(tree, data))
	{
		return FALSE;
	}
	Node* N = (Node*)malloc(sizeof(Node));
	N->data = data;
	N->left = NULL;
	N->right = NULL;
	N->parent = NULL;

	InsertNodeToTree(tree, data, N);

	if (tree->root == N)
	{
		N->color = BLACK;
	}
	else
	{
		paintTreeAfterInsert(tree, N);
	}

	tree->size += 1;

	return TRUE;
}

/**
 *
 * @param tree
 * @param data
 * @param N
 */
void InsertNodeToTree(RBTree* tree, const void* data, Node* N)
{
	if (tree->root == NULL)
	{
		tree->root = N;
	}
	else
	{
		Node* currentNode = tree->root;
		int side;
		while (N->parent == NULL)
		{
			side = getSide(currentNode, data, tree->compFunc);
			if (side == LEFT)
			{
				if (currentNode->left == NULL)
				{
					currentNode->left = N;
					N->parent = currentNode;
					break;
				}
				else
				{
					currentNode = currentNode->left;
				}
			}
			else
			{
				if (currentNode->right == NULL)
				{
					currentNode->right = N;
					N->parent = currentNode;
					break;
				}
				else
				{
					currentNode = currentNode->right;
				}
			}
		}
	}
}

/**
 * remove an item from the tree
 * @param tree: the tree to remove an item from.
 * @param data: item to remove from the tree.
 * @return: 0 on failure, other on success. (if data is not in the tree - failure).
 */
int deleteFromRBTree(RBTree* tree, void* data)
{
	if (tree == NULL || RBTreeContains(tree, data) == FALSE || tree->root == NULL)
	{
		return FALSE;
	}

	Node* M = getNodeByData(tree, data);
	Node* C = NULL;
	if (M->left != NULL)
	{
		C = M->left;
	}
	else if (M->right != NULL)
	{
		C = M->right;
	}
	Node* P = M->parent;
	Node* S = getSister(M, tree->compFunc);

	if (M->color == RED)
	{
		if (P->right == M)
		{
			P->right = NULL;
		}
		else
		{
			P->left = NULL;
		}
		free(M);
		M = NULL;
	}
	else
	{

		void* cData = C->data;
		C->data = M->data;
		M->data = cData;
		if (C->parent->left == C)
		{
			C->parent->left = NULL;
		}
		else
		{
			C->parent->right = NULL;
		}
		Node* tmpNode = C;
		free(tmpNode);
		C = M;
		if (isBlack(C) == FALSE)
		{
			C->color = BLACK;
		}
		else
		{
			DbCase(tree, C, P, S);
		}
	}

	tree->size -= 1;

	return TRUE;
}

/**
 *
 * @param tree
 * @param C
 * @param P
 * @param S
 */
void DbCase(RBTree* tree, Node* C, Node* P, Node* S)
{
	if (tree->root == C)
	{
		return;
	}
	else if (isBlack(S) == TRUE && isBlack(S->left) == TRUE &&
		isBlack(S->right) == TRUE)
	{
		if (isBlack(P) == FALSE)
		{
			P->color = BLACK;
			S->color = RED;
		}
		else
		{
			S->color = RED;
			DbCase(tree, P, P->parent, getSister(P->parent, tree->compFunc));
		}
	}
	else if (isBlack(S) == FALSE)
	{
		S->color = BLACK;
		P->color = RED;
		if (C == P->left)
		{
			leftRotation(tree, P);
		}
		else
		{
			rightRotation(tree, P);
		}
		DbCase(tree, C, P, S);
	}
	else
	{
		Node* Sc;
		Node* Sf;
		if (P->parent->left == P)
		{
			Sc = S->right;
			Sf = S->left;
		}
		else
		{
			Sc = S->left;
			Sf = S->right;
		}
		if (isBlack(Sf) == TRUE && isBlack(Sc) == FALSE)
		{
			Sc->color = BLACK;
			S->color = RED;

			if (P->parent->left == P)
			{
				rightRotation(tree, S);
			}
			else
			{
				leftRotation(tree, S);
			}
		}

		if (isBlack(S) == TRUE && isBlack(Sc) == FALSE)
		{
			S->color = RED;
			P->color = BLACK;

			if (P->left == C)
			{
				leftRotation(tree, P);
			}
			else
			{
				rightRotation(tree, P);
			}
			Sc->color = BLACK;
		}
	}
}

/**
 *
 * @param compareFunc
 * @param node
 * @param data
 * @return
 */
int containsNode(Node* node, const void* data, CompareFunc compareFunc)
{
	if (node == NULL)
	{
		return FALSE;
	}
	int side = getSide(node, data, compareFunc);
	switch (side)
	{
		case RIGHT:
			return containsNode(node->right, data, compareFunc);
		case LEFT:
			return containsNode(node->left, data, compareFunc);
		default:
			return TRUE;
	}
}

/**
 * check whether the tree RBTreeContains this item.
 * @param tree: the tree to check an item in.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int RBTreeContains(const RBTree* tree, const void* data)
{
	return containsNode(tree->root, data, tree->compFunc);
}

/**
 *
 * @param node
 * @param func
 * @param args
 */
void forEachNode(const Node* node, forEachFunc func, void* args)
{
	if (node == NULL)
	{
		return;
	}
	else
	{
		forEachNode(node->left, func, args);
		forEachNode(node->right, func, args);
		func(node, args);
	}
}

/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(const RBTree* tree, forEachFunc func, void* args)
{
	forEachNode(tree->root, func, args);
	return TRUE;
}

/**
 *
 * @param root
 */
void freeNode(Node* node)
{
	if (node == NULL)
	{
		return;
	}
	else
	{
		freeNode(node->right);
		freeNode(node->left);
		free(node);
	}
}

/**
 * free all memory of the data structure.
 * @param tree: pointer to the tree to free.
 */
void freeRBTree(RBTree** tree)
{
	if (tree == NULL)
	{
		return;
	}
	else
	{
		freeNode((*tree)->root);
		free(tree);
	}
}

/**
 *
 * @param a
 * @param b
 * @return
 */
int intComp(const void* a, const void* b)
{
	int* a_int = (int*)a;
	int* b_int = (int*)b;
	return (*a_int - *b_int);
}

int main()
{
	RBTree* tree = newRBTree(&intComp, NULL);

	int data_7 = 7;
	printf("Data: %d\n", data_7);
	insertToRBTree(tree, &data_7);
	printf("Valid: %d\n", isValidRBTree(tree));
	printRBTree(tree->root);

	int data_8 = 8;
	printf("Data: %d\n", data_8);
	insertToRBTree(tree, &data_8);
	printf("Valid: %d\n", isValidRBTree(tree));
	printRBTree(tree->root);

	deleteFromRBTree(tree, &data_7);
	printf("Valid: %d\n", isValidRBTree(tree));
	printRBTree(tree->root);

	int data_6 = 6;
	printf("Data: %d\n", data_6);
	insertToRBTree(tree, &data_6);
	printf("Valid: %d\n", isValidRBTree(tree));
	printRBTree(tree->root);

	printf("Data: %d\n", data_6);
	insertToRBTree(tree, &data_6);
	printf("Valid: %d\n", isValidRBTree(tree));
	printRBTree(tree->root);

	deleteFromRBTree(tree, &data_7);
	printf("Valid: %d\n", isValidRBTree(tree));
	printRBTree(tree->root);

	deleteFromRBTree(tree, &data_6);
	printf("Valid: %d\n", isValidRBTree(tree));
	printRBTree(tree->root);


//	int data_9 = 9;
//	printf("Data: %d\n", data_9);
//	insertToRBTree(tree, &data_9);
//	printf("Valid: %d\n", isValidRBTree(tree));
//	printRBTree(tree->root);
//
//	int data_10 = 10;
//	printf("Data: %d\n", data_10);
//	insertToRBTree(tree, &data_10);
//	printf("Valid: %d\n", isValidRBTree(tree));
//	printRBTree(tree->root);
//
//	int data_12 = 12;
//	printf("Data: %d\n", data_12);
//	insertToRBTree(tree, &data_12);
//	printf("Valid: %d\n", isValidRBTree(tree));
//	printRBTree(tree->root);
//
//	int data_1 = 1;
//	printf("Data: %d\n", data_1);
//	insertToRBTree(tree, &data_1);
//	printf("Valid: %d\n", isValidRBTree(tree));
//	printRBTree(tree->root);
//
//	int data_11 = 11;
//	printf("Data: %d\n", data_11);
//	insertToRBTree(tree, &data_11);
//	printf("Valid: %d\n", isValidRBTree(tree));
//	printRBTree(tree->root);

	freeRBTree(&tree);
	deleteFromRBTree(tree, (void*)&data_7);
}

