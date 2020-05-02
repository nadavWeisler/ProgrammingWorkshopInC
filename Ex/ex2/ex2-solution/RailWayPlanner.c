#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define EMPTY_FILE_ERROR "File is empty.\n"
#define INVALID_INPUT_ERROR "Invalid input in line: %d.\n"
#define FILE_DOESNT_EXIST_ERROR "File doesn't exists.\n"
#define USAGE_ERROR "Usage: RailWayPlanner %d\n"
#define MINIMAL_PRICE_LINE "The minimal price is: %d\n"
#define MAX_LINE_LENGTH 1024
#define MIN_LINE_COUNT 4
#define TRUE 0
#define FALSE 1
#define OPEN_FILE_PERMISSION "r"

struct railway_part
{
	int price;
	int length;
	char right;
	char left;
};

typedef struct railway_part Part;

int validateFileName(char* name[])
{
	return TRUE;
}

int readFile(FILE* input)
{
	printf("Read File\n");
	char line[MAX_LINE_LENGTH];
	while (fgets(line, MAX_LINE_LENGTH, input) != NULL)
	{
		printf("1");
		printf("%s", line);
	}
	free(line);
	return TRUE;
}

int main(int argc, char* argv[])
{
	if (argc > 2)
	{
		printf("%s", USAGE_ERROR);
		return FALSE;
	}
	FILE* input = fopen(argv[1], OPEN_FILE_PERMISSION);
	readFile(input);
	if (input == NULL)
	{
		fclose(input);
		return FALSE;
	}
	fclose(input);
	return TRUE;
}