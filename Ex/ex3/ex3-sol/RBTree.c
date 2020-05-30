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

Node* getUncle(Node* root, CompareFunc compareFunc)
{
	int parentSide = getSide(compareFunc, root->parent, root->data);
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
 *
 * @param root
 * @param newNode
 */
void leftRotation(Node** root, Node* newNode)
{
	Node* rightNode = newNode->right;
	newNode->right = rightNode->left;
	if (newNode->right != NULL)
	{
		newNode->right->parent = newNode;
	}
	rightNode->parent = newNode->parent;
	if (newNode->parent == NULL)
	{
		*root = rightNode;
	}
	else if (newNode == newNode->parent->left)
	{
		newNode->parent->left = rightNode;
	}
	else
	{
		newNode->parent->right = rightNode;
	}
	rightNode->left = newNode;
	newNode->parent = rightNode;
}

/**
 *
 * @param root
 * @param newNode
 */
void rightRotation(Node** root, Node* newNode)
{
	Node* leftNode = newNode->left;
	newNode->left = leftNode->right;
	if (leftNode->right != NULL)
	{
		leftNode->right->parent = newNode;
	}
	leftNode->parent = newNode->parent;
	if (leftNode->parent == NULL)
	{
		*root = leftNode;
	}
	else if (newNode == newNode->parent->left)
	{
		newNode->parent->left = leftNode;
	}
	else
	{
		newNode->parent->right = leftNode;
	}
	leftNode->right = newNode;
	newNode->parent = leftNode;
}

/**
 * To DO
 * @param tree
 * @return
 */
int paintTree(RBTree* tree, Node* newNode)
{
	while (newNode->parent->color != RED && newNode != tree->root)
	{
		Node* uncle = getUncle(tree->root, newNode->data);
		if (uncle->color == RED)
		{
			uncle->color = BLACK;
			newNode->parent->color = BLACK;
			newNode->parent->parent->color = RED;
			newNode = newNode->parent->parent;
		}
		else
		{
			// Left-Left (LL) case, do following
			// (i)  Swap color of parent and grandparent
			// (ii) Right Rotate Grandparent
			if (newNode->parent == newNode->parent->parent->left &&
				newNode == newNode->parent->left)
			{
				Color parentColor = newNode->parent->color;
				newNode->parent->color = newNode->parent->parent->color;
				newNode->parent->parent->color = parentColor;
				rightRotation(&(tree->root), newNode->parent->parent);
			}

			// Left-Right (LR) case, do following
			// (i)  Swap color of current node  and grandparent
			// (ii) Left Rotate Parent
			// (iii) Right Rotate Grand Parent
			if (newNode->parent == newNode->parent->parent->left &&
				newNode == newNode->parent->right)
			{
				Color newNodeColor = newNode->color;
				newNode->color = newNode->parent->parent->color;
				newNode->parent->parent->color = newNodeColor;
				leftRotation(&(tree->root), newNode->parent);
				rightRotation(&(tree->root), newNode->parent->parent);
			}

			// Right-Right (RR) case, do following
			// (i)  Swap color of parent and grandparent
			// (ii) Left Rotate Grandparent
			if (newNode->parent == newNode->parent->parent->right &&
				newNode == newNode->parent->right)
			{
				Color parentColor = newNode->parent->color;
				newNode->parent->color = newNode->parent->parent->color;
				newNode->parent->parent->color = parentColor;
				leftRotation(&(tree->root), newNode->parent->parent);
			}

			// Right-Left (RL) case, do following
			// (i)  Swap color of current node  and grandparent
			// (ii) Right Rotate Parent
			// (iii) Left Rotate Grand Parent
			if (newNode->parent == newNode->parent->parent->right &&
				newNode == newNode->parent->left)
			{
				char currentColor = newNode->color;
				newNode->color = newNode->parent->parent->color;
				newNode->parent->parent->color = currentColor;
				rightRotation(&(tree->root), newNode->parent);
				leftRotation(&(tree->root), newNode->parent->parent);
			}
		}
	}
	tree->root->color = BLACK;
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
 *
 * @param tree
 * @param data
 * @return
 */
Node* getParentOfData(RBTree* tree, void* data)
{
	Node* root = tree->root;
	Node* returnValue = NULL;
	while (root != NULL)
	{
		returnValue = root;
		if (data < root->data)
			root = root->left;
		else
			root = root->right;
	}
	return returnValue;
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
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->data = data;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->parent = NULL;
	if (tree->root == NULL)
	{
		newNode->color = BLACK;
		tree->root = newNode;
	}
	else
	{
		newNode->color = RED;
		Node* newNodeParent = getParentOfData(tree, data);
		newNode->parent = newNodeParent;
		switch (getSide(tree->compFunc, newNodeParent, data))
		{
			case LEFT:
				newNodeParent->left = newNode;
				break;
			case RIGHT:
				newNodeParent->right = newNode;
				break;
			default:
				returnVal = FALSE;
				break;
		}
		paintTree(tree, newNode);
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
void freeNode(Node* root)
{
	if (root == NULL)
	{
		return;
	}
	else
	{
		freeNode(root->right);
		freeNode(root->left);
		free(root);
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
	printf("Hello, World!\n");
	int a = 2;
	insertToRBTree(tree, &a);
	printRBTree(tree->root);
	freeRBTree(&tree);
	return 0;
}

