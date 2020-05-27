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

/**
 *
 * @param compFunc
 * @param root
 * @param data
 * @return
 */
int getSide(CompareFunc compFunc, Node* root, void* data)
{
	int val = compFunc(data, root->data);
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

Node* getUncle(Node* root, void* data, CompareFunc compareFunc)
{
	int parentSide = getSide(compareFunc, root->parent, data);
	switch (parentSide)
	{
		case RIGHT:
			return root->parent->parent->left;
		case LEFT:
			return root->parent->parent->right;
		default:
			return NULL;
	}
}

/**
 * To DO
 * @param tree
 * @return
 */
int fixColor(RBTree* tree)
{
	return TRUE;
}

/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
RBTree* newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
	RBTree* new_tree = (RBTree*)malloc(sizeof(RBTree*));
	new_tree->root = NULL;
	new_tree->freeFunc = freeFunc;
	new_tree->compFunc = compFunc;
	return new_tree;
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int insertToRBTree(RBTree* tree, void* data)
{
	int returnVal = TRUE;
	Node* newNode = (Node*)malloc(sizeof(Node*));
	newNode->data = data;
	newNode->color = RED;
	if (tree->root == NULL)
	{
		newNode->color = BLACK;
		tree->root = newNode;
	}
	else if (tree->root->color == BLACK)
	{
		int side = getSide(tree->compFunc, tree->root, data);
		switch (side)
		{
			case (LEFT):
				tree->root->left = newNode;
				break;
			case (RIGHT):
				tree->root->right = newNode;
				break;
			default:
				returnVal = FALSE;
		}
	}
	return returnVal;
}

/**
 * remove an item from the tree
 * @param tree: the tree to remove an item from.
 * @param data: item to remove from the tree.
 * @return: 0 on failure, other on success. (if data is not in the tree - failure).
 */
int deleteFromRBTree(RBTree* tree, void* data)
{
	return TRUE;
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
	int side = getSide(compareFunc, node, &data);
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
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(const RBTree* tree, forEachFunc func, void* args)
{
	return TRUE;
}

/**
 * free all memory of the data structure.
 * @param tree: pointer to the tree to free.
 */
void freeRBTree(RBTree** tree)
{

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
	printf("Hello, World!\n");
	int a = 2;
	insertToRBTree(tree, &a);
	printRBTree(tree->root);
	return 0;
}

