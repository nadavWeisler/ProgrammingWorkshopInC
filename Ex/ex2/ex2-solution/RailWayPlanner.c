#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define EMPTY_FILE_ERROR "File is empty."
#define INVALID_INPUT_ERROR "Invalid input in line: %d."
#define FILE_DOESNT_EXIST_ERROR "File doesn't exists."
#define USAGE_ERROR "Usage: RailWayPlanner <InputFile>"
#define OUTPUT_FILE_PATH "railway_planner_output.txt"
#define MINIMAL_PRICE_LINE "The minimal price is: %d"
#define MAX_LINE_LENGTH 1024
#define TRUE 0
#define FALSE 1
#define OPEN_FILE_PERMISSION "r"
#define WRITE_FILE_PERMISSION "w"
#define EMPTY_CHAR '\0'
#define LENGTH_INDEX 0
#define PARTS_COUNT_INDEX 1
#define COMMA ","
#define DEFAULT_MIN_VALUE -1
#define PART_PRICE_INDEX 3
#define PART_LENGTH_INDEX 2
#define PART_RIGHT_INDEX 1
#define PART_LEFT_INDEX 0

/**
 * Railway part struct
 */
typedef struct railwayPart
{
	int price;
	int length;
	char right;
	char left;
} RailwayPart;

/**
 * One direction struct
 */
typedef struct direction
{
	int price;
	char right;
} Direction;

int railwayLength;
int railwaySignsCount;
int partCount = 0;
char* signs;
RailwayPart* parts;

/**
 * Split signs line
 * @param line			Line input
 * @return				TRUE if succeeded, FALSE otherwise
 */
int splitSigns(char* line)
{
	char* newField = strtok(line, COMMA);
	signs = (char*)malloc(0);
	int signIndex = 0;
	while (newField != NULL)
	{
		if (signIndex > railwaySignsCount)
		{
			break;
		}
		signs = (char*)realloc(signs, (1 + signIndex) * sizeof(char));
		signs[signIndex] = newField[0];
		signIndex++;
		newField = strtok(NULL, COMMA);
	}
	if (signIndex != railwaySignsCount)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

/**
 * Validate string is numeric
 * @param param			Param to validate
 * @param min			Min value
 * @param paramInt		converted int
 * @return				TRUE if succeeded, FALSE otherwise
 */
int validateNumeric(char* param, const int min, int* paramInt)
{
	char* literal;
	double digitNum = strtod(param, &literal);
	if (min > digitNum || (int)digitNum != digitNum)
	{
		return FALSE;
	}
	*paramInt = (int)digitNum;
	return TRUE;
}

/**
 *	split part line
 * @param line		part line
 * @param part		pointer for part
 * @return			TRUE if succeeded, FALSE otherwise
 */
int splitPart(char* line, RailwayPart* part)
{
	char* newField = strtok(line, COMMA);
	int paramIndex = 0;
	while (newField != NULL)
	{
		int intValue = 0;
		switch (paramIndex)
		{
			case PART_LEFT_INDEX:
				part->left = newField[0];
				break;
			case PART_RIGHT_INDEX:
				part->right = newField[0];
				break;
			case PART_LENGTH_INDEX:
				if (validateNumeric(newField, 1, &intValue) == TRUE)
				{
					part->length = intValue;
				}
				else
				{
					return FALSE;
				}
				break;
			case PART_PRICE_INDEX:
				if (validateNumeric(newField, 1, &intValue) == TRUE)
				{
					part->price = intValue;
				}
				else
				{
					return FALSE;
				}
				break;
			default:
				return FALSE;
		}
		paramIndex++;
		newField = strtok(NULL, COMMA);
	}
	return TRUE;
}

/**
 *	Get line count for file
 * @param fileName		file path
 * @return				line count
 */
int getLineCount(const char* fileName)
{
	FILE* input = fopen(fileName, OPEN_FILE_PERMISSION);
	int currentLine = 0;
	int number_of_lines = 0;
	while (currentLine != EOF)
	{
		currentLine = fgetc(input);
		number_of_lines++;
	}
	return number_of_lines;
}

/**
 *	Read input file
 * @param input			input file
 * @param lineCount		input file line count
 * @return				TRUE if succeeded, FALSE otherwise
 */
int readFile(FILE* input, const int lineCount)
{
	char line[MAX_LINE_LENGTH];
	int lineIndex = 0;
	RailwayPart newPart;
	parts = (RailwayPart*)malloc(sizeof(RailwayPart) * (lineCount - 4));
	while (fgets(line, MAX_LINE_LENGTH, input) != NULL)
	{
		line[strlen(line) - 1] = EMPTY_CHAR;
		if (strlen(line) == 0)
		{
			continue;
		}
		else if (lineIndex == LENGTH_INDEX)
		{
			int intValue;
			if (validateNumeric(line, 0, &intValue) == TRUE)
			{
				railwayLength = intValue;
			}
			else
			{
				return FALSE;
			}
		}
		else if (lineIndex == PARTS_COUNT_INDEX)
		{
			int intValue;
			if (validateNumeric(line, 1, &intValue) == TRUE)
			{
				railwaySignsCount = intValue;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			if (lineIndex == 2)
			{
				signs = (char*)malloc(railwaySignsCount * sizeof(char));
				splitSigns(line);
			}
			else
			{
				if (splitPart(line, &newPart) == TRUE)
				{
					parts[partCount] = newPart;
					partCount++;
				}
				else
				{
					return FALSE;
				}
			}
		}
		lineIndex++;
	}

	if (lineIndex == 0)
	{
		return FALSE;
	}

	return TRUE;
}

/**
 * reset direction matrix
 * @param railwaysCostMat	directions matrix
 */
void resetRailwayCostMatrix(Direction*** railwaysCostMat)
{
	for (int i = 0; i <= railwayLength; i++)
	{
		for (int signIndex = 0; signIndex < railwaySignsCount; signIndex++)
		{
			if (i == 0)
			{
				Direction zeroDirection;
				zeroDirection.price = 0;
				zeroDirection.right = signs[signIndex];
				(*railwaysCostMat)[i][signIndex] = zeroDirection;
			}
			else
			{
				Direction newDirection;
				newDirection.price = INT_MAX;
				newDirection.right = signs[signIndex];
				(*railwaysCostMat)[i][signIndex] = newDirection;
			}
		}
	}
}

/**
 * Get sign index
 * @param sign 			sign
 * @return 				sign index, -1 if not exist
 */
int getIndexFromSign(char sign)
{
	for (int i = 0; i < railwaySignsCount; i++)
	{
		if (signs[i] == sign)
		{
			return i;
		}
	}
	return -1;
}

/**
 *	Fill railwaysCostMat matrix
 *	@param railwaysCostMat 			pointer to directions matrix
 */
void fillPriceMatrix(Direction*** railwaysCostMat)
{
	for (int lengthIndex = 1; lengthIndex <= railwayLength; lengthIndex++)
	{
		for (int signIndex = 0; signIndex < railwaySignsCount; signIndex++)
		{
			for (int partIndex = 0; partIndex < partCount; partIndex++)
			{
				const RailwayPart addedPart = parts[partIndex];
				if ((*railwaysCostMat)[lengthIndex][signIndex].right != addedPart.left
					|| addedPart.length > lengthIndex)
				{
					continue;
				}
				int addedPartStartIndex = getIndexFromSign(addedPart.right);
				if (addedPartStartIndex == -1)
				{
					continue;
				}
				Direction prevPart = (*railwaysCostMat)[lengthIndex - addedPart.length][addedPartStartIndex];
				if (prevPart.price == INT_MAX)
				{
					continue;
				}
				int newPrice = prevPart.price + addedPart.price;
				if (newPrice < (*railwaysCostMat)[lengthIndex][signIndex].price)
				{
					(*railwaysCostMat)[lengthIndex][signIndex].price = newPrice;
				}
			}
		}
	}
}

/**
 *	Get min price from railway array
 * @param railwaysCostMat		railway directions matrix
 * @return						min price
 */
int getMinPriceInList(Direction** railwaysCostMat)
{
	int minVal = INT_MAX;
	for (int i = 0; i < railwaySignsCount; i++)
	{
		if (railwaysCostMat[railwayLength][i].price < minVal)
		{
			minVal = railwaysCostMat[railwayLength][i].price;
		}
	}
	return minVal;
}

/**
 *	Get lowest railway price
 * @return		lowest railway price (int)
 */
int getLowestRailwayPrice()
{
	Direction** railwaysCostMat = (Direction**)malloc(sizeof(Direction*) * (railwayLength + 1));
	for (int i = 0; i <= railwayLength; i++)
	{
		railwaysCostMat[i] = (Direction*)malloc((railwaySignsCount) * sizeof(Direction));
	}
	resetRailwayCostMatrix(&railwaysCostMat);
	fillPriceMatrix(&railwaysCostMat);
	int minVal = getMinPriceInList(railwaysCostMat);
	for (int i = 0; i < railwayLength; i++)
	{
		free(railwaysCostMat[i]);
		railwaysCostMat[i] = NULL;
	}
	free(railwaysCostMat);
	railwaysCostMat = NULL;
	if (minVal == INT_MAX)
	{
		return DEFAULT_MIN_VALUE;
	}
	else
	{
		return minVal;
	}
}

/**
 *	Write output file
 * @param outputFilePath	output file path
 * @param text				text to right
 * @param val				param to write
 */
void writeOutputFile(char* outputFilePath, char* text, const int val)
{
	FILE* outputFile = fopen(outputFilePath, WRITE_FILE_PERMISSION);
	if (val == INT_MIN)
	{
		fprintf(outputFile, "%s", text);
	}
	else
	{
		fprintf(outputFile, text, val);
	}
	fclose(outputFile);
}

/**
 *	Free main memory
 */
void freeMain()
{
	free(signs);
	free(parts);
}

/**
 *	Main Function
 * @param argc 		argument count
 * @param argv 		arguments
 * @return			EXIT_SUCCESS is succeeded EXIT_FAILURE otherwise
 */
int main(int argc, char* argv[])
{
	int exitCode = EXIT_SUCCESS;
	if (argc > 2)
	{
		writeOutputFile(OUTPUT_FILE_PATH, USAGE_ERROR, INT_MIN);
		exitCode = EXIT_FAILURE;
	}
	FILE* input = fopen(argv[1], OPEN_FILE_PERMISSION);
	if (input == NULL)
	{
		writeOutputFile(OUTPUT_FILE_PATH, FILE_DOESNT_EXIST_ERROR, INT_MIN);
		exitCode = EXIT_FAILURE;
	}
	else
	{
		int lineCount = getLineCount(argv[1]);
		if (lineCount == 0)
		{
			writeOutputFile(OUTPUT_FILE_PATH, EMPTY_FILE_ERROR, INT_MIN);
			exitCode = EXIT_FAILURE;
		}
		if (readFile(input, lineCount) == TRUE)
		{
			int price = getLowestRailwayPrice();
			writeOutputFile(OUTPUT_FILE_PATH, MINIMAL_PRICE_LINE, price);
		}
		else
		{
			writeOutputFile(OUTPUT_FILE_PATH, INVALID_INPUT_ERROR, INT_MIN);
			exitCode = EXIT_FAILURE;
		}
		fclose(input);
	}
	freeMain();
	return exitCode;
}