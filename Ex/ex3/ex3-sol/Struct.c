#include <stdio.h>
#include <stdlib.h>
#include "RBTree.c"
#include "Structs.h"

int stringCompare(const void* a, const void* b)
{
	char* aChar = (char *)a;
	char* bChar = (char *)b;
	strcmp(aChar, bChar);
}

int concatenate(const void* word, void* pConcatenated)
{

}

void freeString(void* s)
{
	char* sChar = (char*)s;
	free(sChar);
}

double getVectorNorm(Vector* vector)
{
	int sum = 0;
	for(int i = 0; i < vector->len; i++)
	{
		sum += pow(vector->vector[i]);
	}
	return sqrt(sum);

}

int vectorCompare1By1(const void* a, const void* b)
{

}

void freeVector(void* pVector)
{

}

int copyIfNormIsLarger(const void* pVector, void* pMaxVector)
{

}

Vector* findMaxNormVectorInTree(RBTree* tree)
{

}