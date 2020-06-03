#include <stdio.h>
#include <stdlib.h>
#include "RBTree.h"

#define TRUE 1
#define FALSE 0

#define RIGHT 1
#define LEFT -1
#define MIDDLE 0
#define NONE -2

/**
 * Get side
 * @param node 			Node pointer
 * @param data 			data
 * @param compFunc 		compare function
 * @return RIGHT LEFT MIDDLE or NONE
 */
int getSide(Node* node, const void* data, CompareFunc compFunc)
{
	if (node == NULL)
	{
		return NONE;
	}
	int val = compFunc(data, node->data);
	if (val > MIDDLE)
	{
		return RIGHT;
	}
	else if (val < MIDDLE)
	{
		return LEFT;
	}
	else
	{
		return MIDDLE;
	}
}

/**
 * Insert Node to tree
 * @param tree 			tree pointer
 * @param data 			data
 * @param N 			node pointer
 */
void insertNodeToTree(RBTree* tree, const void* data, Node* N)
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
 * Check if node is black
 * @param node 			node pointer
 * @return 				TRUE if black or NULL, FALSE otherwise
 */
int isBlack(Node* node)
{
	return (node == NULL || node->color == BLACK);
}

/**
 * Get node sibling
 * @param node 			node pointer
 * @param compareFunc 	compare function
 * @return 				node pointer
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
 *	left rotation
 * @param tree			tree pointer
 * @param node			node pointer
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
 *	right pointer
 * @param tree			tree pointer
 * @param node			node pointer
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
 *	Deals with double black
 * @param tree			tree pointer
 * @param C				node pointer
 * @param P				node pointer
 * @param S				node pointer
 */
void doubleBlack(RBTree* tree, Node* C, Node* P, Node* S)
{
	if (tree->root == C)
	{
		return;
	}
	else if (isBlack(S) == TRUE && isBlack(S->left) == TRUE && isBlack(S->right) == TRUE)
	{
		if (isBlack(P) == FALSE)
		{
			P->color = BLACK;
			S->color = RED;
		}
		else
		{
			S->color = RED;
			doubleBlack(tree, P, P->parent, getSister(P->parent, tree->compFunc));
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
		doubleBlack(tree, C, P, S);
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
 * Get minimum node
 * @param node 			node pointer
 * @return				node pointer of min Node
 */
Node* getMinimum(Node* node)
{
	if (node->left != NULL)
	{
		return getMinimum(node->left);
	}
	return node;
}
/**
 * Get node successor
 * @param node 			node pointer
 * @return 				node pointer of successor
 */
Node* getSuccessor(Node* node)
{
	if (node->right == NULL)
	{
		return node->left;
	}
	return getMinimum(node->right);
}

/**
 * Paint tree after insert
 * @param tree 			tree pointer
 * @param N 			node pointer
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
				rightRotation(tree, G);
				P = N;
			}
			else if (P == G->right && N == P->left)
			{
				rightRotation(tree, P);
				leftRotation(tree, G);
				P = N;
			}
			else if (P == G->right && N == P->right)
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
 * Get node pointer by data
 * @param tree 			tree pointer
 * @param data 			data
 * @return 				node pointer
 */
Node* getNodeByData(const RBTree* tree, const void* data)
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
 * Create new RBTree
 * @param compFunc 		compare function
 * @param freeFunc 		free function
 * @return 				tree pointer
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
 * Insert data to RBTree
 * @param tree 			tree pointer
 * @param data 			data
 * @return 				TRUE if worked, FALSE otherwise
 */
int insertToRBTree(RBTree* tree, void* data)
{
	if (getNodeByData(tree, data) != NULL)
	{
		return FALSE;
	}

	Node* N = (Node*)malloc(sizeof(Node));
	N->data = data;
	N->left = NULL;
	N->right = NULL;
	N->parent = NULL;
	N->color = RED;

	insertNodeToTree(tree, data, N);

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
 * Delete from RBTree
 * @param tree 			tree pointer
 * @param data 			data
 * @return 				TRUE if worked, FALSE otherwise
 */
int deleteFromRBTree(RBTree* tree, void* data)
{
	if (tree == NULL || RBTreeContains(tree, data) == FALSE || tree->root == NULL)
	{
		return FALSE;
	}

	Node* M = getNodeByData(tree, data);
	Node* C = getSuccessor(M);
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
			doubleBlack(tree, C, P, S);
		}
	}

	tree->size -= 1;
	return TRUE;
}

/**
 * Check if data exist in node.
 * @param node 			node pointer
 * @param data 			data
 * @param compareFunc 	compare function
 * @return 				TRUE if exist, FALSE otherwise
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
 * Check if data exist in tree
 * @param tree 			tree pointer
 * @param data 			data
 * @return 				TRUE if exist, FALSE otherwise
 */
int RBTreeContains(const RBTree* tree, const void* data)
{
	return getNodeByData(tree, data) != NULL;
}

/**
 * Do func on node
 * @param node 			node pointer
 * @param func 			for each function
 * @param args 			arguments
 * @return 				TRUE if worked, FALSE otherwise
 */
int forEachNode(Node* node, forEachFunc func, void* args)
{
	if (node != NULL)
	{
		if (!func(node, args))
		{
			return FALSE;
		}
		return forEachNode(getSuccessor(node), func, args);
	}
	return TRUE;
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
	return forEachNode(getMinimum(tree->root), func, args);
}

/**
 * Free node
 * @param node				node pointer
 * @param freeFunc 			free function
 */
void freeNode(Node* node, FreeFunc freeFunc)
{
	if (node != NULL)
	{
		freeNode(node->left, freeFunc);
		freeNode(node->right, freeFunc);
		node->data = NULL;
		free(node);
		node = NULL;
	}
}

/**
 * free all memory of the data structure.
 * @param tree: pointer to the tree to free.
 */
void freeRBTree(RBTree** tree)
{
	if (tree != NULL)
	{
		RBTree* pTree = *tree;
		freeNode(pTree->root, pTree->freeFunc);
		free(pTree);
		pTree = NULL;
	}
}
