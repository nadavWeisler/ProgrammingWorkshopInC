#include <stdlib.h>
#include "RBTree.h"
#include "Structs.h"
#include <string.h>

#define NEW_LINE "\n"
#define BIGGER 1
#define EQUAL 0
#define SMALLER -1

#define TRUE 1
#define FALSE 0

/**
 * Get minimal value
 * @param n1 		double
 * @param n2 		double
 * @return 			minimum
 */
int getMinValue(int n1, int n2)
{
	if (n1 <= n2)
	{
		return n1;
	}
	else
	{
		return n2;
	}
}

/**
 * compare two strings
 * @param a			first string
 * @param b			second string
 * @return			result
 */
int stringCompare(const void* a, const void* b)
{
	char* aChar = (char*)a;
	char* bChar = (char*)b;
	size_t min;
	if (strlen(aChar) <= strlen(bChar))
	{
		min = strlen(aChar);
	}
	else
	{
		min = strlen(bChar);
	}
	return strncmp(a, b, min);
}

/**
 * concatenate two strings
 * @param word				string
 * @param pConcatenated		string
 * @return					concat result
 */
int concatenate(const void* word, void* pConcatenated)
{
	if ((strcat(pConcatenated, word) == NULL) || (strcat(pConcatenated, NEW_LINE) == NULL))
	{
		return FALSE;
	}
	return TRUE;
}

/**
 * free string
 * @param s					string
 */
void freeString(void* s)
{
	free((char*)s);
}

/**
 * Vector compare
 * @param a		first vector
 * @param b		second vector
 * @return		compare result
 */
int vectorCompare1By1(const void* a, const void* b)
{
	Vector* aVector = (Vector*)a;
	Vector* bVector = (Vector*)b;
	int aVectorLength = aVector->len;
	int bVectorLength = bVector->len;
	int minLen = getMinValue(aVectorLength, bVectorLength);
	for (int i = 0; i < minLen; i++)
	{
		double aValue = aVector->vector[i];
		double bValue = bVector->vector[i];
		if (aValue > bValue)
		{
			return BIGGER;
		}
		else if (aValue == bValue)
		{
			return EQUAL;
		}
		else
		{
			return SMALLER;
		}
	}

	if (aVectorLength > bVectorLength)
	{
		return BIGGER;
	}
	else if (aVectorLength == bVectorLength)
	{
		return EQUAL;
	}
	else
	{
		return SMALLER;
	}
}

/**
 * free vector
 * @param pVector		vector pointer
 */
void freeVector(void* pVector)
{
	Vector* vector = (Vector*)pVector;
	free(vector->vector);
	free(vector);
}

/**
 * get vector norm
 * @param vector		vector pointer
 * @return				vector norm
 */
double getVectorDoubleNorm(Vector* vector)
{
	double sum = 0.0;
	for (int i = 0; i < vector->len; i++)
	{
		sum += ((vector->vector[i], 2) * (vector->vector[i], 2));
	}
	return sum;
}

/**
 * copy norm if larger
 * @param pVector			vector pointer
 * @param pMaxVector		vector pointer
 * @return					result
 */
int copyIfNormIsLarger(const void* pVector, void* pMaxVector)
{
	if (pVector == NULL || pMaxVector == NULL)
	{
		return FALSE;
	}
	else
	{
		Vector* vector = (Vector*)pVector;
		Vector* maxVector = (Vector*)pMaxVector;
		double vectorNorm = getVectorDoubleNorm(vector);
		double maxVectorNorm = getVectorDoubleNorm(maxVector);
		if (vectorNorm > maxVectorNorm)
		{
			maxVector->len = vector->len;
			maxVector->vector = realloc(maxVector->vector, (maxVector->len * sizeof(double)));
			if (maxVector->vector == NULL)
			{
				return FALSE;
			}
			for (int i = 0; i < maxVector->len; i++)
			{
				maxVector->vector[i] = vector->vector[i];
			}
		}
		return TRUE;
	}
}

/**
 * find max norm vector
 * @param tree				tree pointer
 * @return					max norm vector pointer
 */
Vector* findMaxNormVectorInTree(RBTree* tree)
{
	Vector* vector = (Vector*)malloc(sizeof(Vector));
	if (vector != NULL)
	{
		vector->len = 0;
		vector->vector = NULL;
		forEachRBTree(tree, copyIfNormIsLarger, vector);
	}
	return vector;
}