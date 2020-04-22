#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_LENGTH 60
#define MAX_STUDENTS 5500
#define PARAM_COUNT 6

#define ID_LENGTH 10
#define FIELD_SIZE 43
#define MIN_AGE 18
#define MAX_AGE 120
#define MIN_GRADE 0
#define MAX_GRADE 100
#define INVALID_VALUE_FLOAT -1.0
#define INVALID_VALUE_INT -1

#define TRUE 0
#define FALSE 1

#define ID_PARAM 0
#define NAME_PARAM 1
#define GRADE_PARAM 2
#define AGE_PARAM 3
#define COUNTRY_PARAM 4
#define CITY_PARAM 5

#define WHITE_SPACE ' '
#define COMMA ","
#define EMPTY_CHAR '\0'
#define HYPHEN '-'
#define NEW_LINE "\n"

#define BEST_STUDENT_TEST "best student info is:"
#define STOP "q\n"
#define START_ERROR_MSG "ERROR:"
#define INVALID_ERROR "Invalid value for"
#define IN_LINE "in line"
#define INVALID_PARAM_COUNT "invalid param count"
#define ASK_INPUT "Enter student info. To exit press q, then enter‬‬"
#define USAGE_ERROR "USAGE: You should write best, merge or quick, come on :("

#define BEST "best"
#define MERGE "merge"
#define QUICK "quick"

/**
 * Stuck for student
 */
struct Student
{
	char id[ID_LENGTH + 1];
	char name[FIELD_SIZE];
	int grade;
	int age;
	char country[FIELD_SIZE];
	char city[FIELD_SIZE];
};

typedef struct Student Student;

int studentsCount = 0;
Student allStudents[MAX_STUDENTS];
char PARAMS_STRING[6][7] = { "Id", "Name", "Grade", "Age", "Country", "City" };

/**
 * Validate that String is int
 * @param param             String - char array
 * @param length            Param length
 * @return                  0 if int, 1 otherwise
 */
int stringIsInt(char* param, int length)
{
	for (int i = 0; i < length; i++)
	{
		if (param[i] == EMPTY_CHAR)
		{
			break;
		}
		else
		{
			unsigned char current = (unsigned char)param[i];
			unsigned int isDigit = (unsigned int)isdigit(current);
			if (isDigit)
			{
				continue;
			}
			return FALSE;
		}
	}
	return TRUE;
}

/**
 * Validate numbers by check if it is number and between minValue and maxValue
 * @param param             Param to check
 * @param minValue          Minimal Value
 * @param maxValue          Maximum Value
 * @return                  Return 0 if true, 1 otherwise
 */
int validateNumber(char* param, int minValue, int maxValue)
{
	if (stringIsInt(param, FIELD_SIZE) == FALSE)
	{
		return FALSE;
	}
	int intParam = atoi(param);
	if (intParam > maxValue || intParam < minValue)
	{
		return FALSE;
	}
	return TRUE;
}

/**
 * Validate string bestValue
 * @param param         Param to validate
 * @param length        Length to iterate
 * @return              0 is True, 1 otherwise
 */
int validateString(char* param, int length)
{
	for (int i = 0; i < length; i++)
	{
		unsigned char current = (unsigned char)param[i];
		unsigned int isAlpha = (unsigned int)isalpha(current);
		unsigned int isSpace = (unsigned int)isspace(current);
		if (param[i] == EMPTY_CHAR)
		{
			break;
		}
		else if (!(isAlpha || param[i] == HYPHEN || isSpace))
		{
			return FALSE;
		}
	}
	return TRUE;
}

/**
 * Get string length
 * @param str           String reference
 * @param maxSize       Max string size
 * @return              String length
 */
int getStringLength(char* str, int maxSize)
{
	int result = 0;
	for (int i = 0; i < maxSize; i++)
	{
		if (str[i] == EMPTY_CHAR || str[i] == '\n')
		{
			break;
		}
		else
		{
			result++;
		}
	}
	return result;
}

/**
 * Validate ID
 * @param param     Param to check
 * @return          Return 0 if true, 1 otherwise
 */
int validateId(char* param)
{
	if (getStringLength(param, FIELD_SIZE) != ID_LENGTH || stringIsInt(param, FIELD_SIZE) == FALSE)
	{
		return FALSE;
	}
	if (param[0] == '0')
	{
		return FALSE;
	}
	return TRUE;
}

/**
 * Validate name param
 * @param param         Param to check
 * @return              Return 0 if true, 1 otherwise
 */
int validateName(char* param)
{
	return validateString(param, FIELD_SIZE);
}

/**
 * Validate age param
 * @param param     Param to check
 * @return           Return 0 if true, 1 otherwise
 */
int validateAge(char* param)
{
	return validateNumber(param, MIN_AGE, MAX_AGE);
}

/**
 * Validate grade param
 * @param param     Param to check
 * @return           Return 0 if true, 1 otherwise
 */
int validateGrade(char* param)
{
	return validateNumber(param, MIN_GRADE, MAX_GRADE);
}

/**
 * Validate city param
 * @param param     Param to check
 * @return           Return 0 if true, 1 otherwise
 */
int validateCity(char* param)
{
	return validateString(param, FIELD_SIZE);
}

/**
 * Validate country param
 * @param param     Param to check
 * @return           Return 0 if true, 1 otherwise
 */
int validateCountry(char* param)
{
	return validateString(param, FIELD_SIZE);
}

/**
 * Validate param
 * @param param        Param to check
 * @param paramType    Param type
 * @return             Return 0 if true, 1 otherwise
 */
int validateParam(char* param, int paramType)
{
	switch (paramType)
	{
		case ID_PARAM:
			return validateId(param);
		case NAME_PARAM:
			return validateName(param);
		case AGE_PARAM:
			return validateAge(param);
		case GRADE_PARAM:
			return validateGrade(param);
		case COUNTRY_PARAM:
			return validateCountry(param);
		case CITY_PARAM:
			return validateCity(param);
		default:
			return FALSE;
	}
}

/**
 * Reset string
 * @param str       Char array
 * @param length    Array length
 */
void resetString(char* str, int length)
{
	for (int i = 0; i < length; i++)
	{
		str[i] = EMPTY_CHAR;
	}
}

/**
 *  Add param to student
 * @param student       Student pointer
 * @param paramVal      Param bestValue
 * @param paramType     Param Type
 */
void addParamToStudent(Student* student, char* paramVal, int paramType)
{
	int intParamVal = 0;
	switch (paramType)
	{
		case ID_PARAM:
			strcpy(student->id, paramVal);
			break;
		case NAME_PARAM:
			strcpy(student->name, paramVal);
			break;
		case AGE_PARAM:
			sscanf(paramVal, "%d", &intParamVal);
			student->age = intParamVal;
			break;
		case GRADE_PARAM:
			sscanf(paramVal, "%d", &intParamVal);
			student->grade = intParamVal;
			break;
		case COUNTRY_PARAM:
			strcpy(student->country, paramVal);
			break;
		case CITY_PARAM:
			strcpy(student->city, paramVal);
			break;
	}
}

/**
 * Get best student index from allstudents array
 * @return      Best student index
 */
int getBestStudent()
{
	float bestValue = INVALID_VALUE_FLOAT;
	int index = INVALID_VALUE_INT;
	for (int i = 0; i < studentsCount; i++)
	{
		float grade = (float)allStudents[i].grade;
		float age = (float)allStudents[i].age;
		if ((grade / age) > bestValue)
		{
			bestValue = grade / age;
			index = i;
		}
	}
	return index;
}

/**
 *  Print best student
 */
void printBestStudent()
{
	int index = getBestStudent();
	if (index > INVALID_VALUE_INT)
	{
		printf("%s %s,%s,%d,%d,%s,%s%s", BEST_STUDENT_TEST,
			allStudents[index].id, allStudents[index].name,
			allStudents[index].grade, allStudents[index].age,
			allStudents[index].country, allStudents[index].city, NEW_LINE);
	}
}

/**
 * Print all students
 */
void printAllStudents()
{
	for (int i = 0; i < studentsCount; i++)
	{
		printf("%s,%s,%d,%d,%s,%s%s",
			allStudents[i].id, allStudents[i].name,
			allStudents[i].grade, allStudents[i].age,
			allStudents[i].country, allStudents[i].city, NEW_LINE);
	}
}

/**
 * Update fields to existing student
 * @param paramCount        Param count (0-5)
 * @param field             Field value
 * @param newStudent        Student pointer
 * @return                  0 if worked, 1 otherwise
 */
int updateFieldsInStudent(int paramCount, char* field, Student* newStudent)
{
	if (validateParam(field, paramCount) == TRUE)
	{
		addParamToStudent(newStudent, field, paramCount);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
 * Generate input line to student
 * @param newStudent        Student pointer
 * @param errorMsg          Error message pointer
 * @param inputLine         Input line (char array)
 * @param lineLength        Input line length
 * @param lineCount         Lines count
 * @return                  0 if worked, 1 otherwise
 */
int inputLineToStudent(Student* newStudent, char* inputLine, int lineCount)
{
	int returnValue = TRUE;
	char* newField;
	newField = strtok(inputLine, COMMA);
	int paramIndex = 0;
	char fields[PARAM_COUNT][FIELD_SIZE];
	while (newField != NULL)
	{
		if (paramIndex >= PARAM_COUNT)
		{
			paramIndex++;
			break;
		}
		strcpy(fields[paramIndex], newField);
		paramIndex++;
		newField = strtok(NULL, COMMA);
	}

	if (paramIndex != PARAM_COUNT)
	{
		printf("%s %s%s", START_ERROR_MSG, INVALID_PARAM_COUNT, NEW_LINE);
		printf("%s %d%s", IN_LINE, lineCount, NEW_LINE);
		returnValue = FALSE;
	}
	else
	{
		for (int i = 0; i < PARAM_COUNT; i++)
		{
			if (updateFieldsInStudent(i, fields[i], newStudent) == FALSE)
			{
				printf("%s %s %s%s", START_ERROR_MSG, INVALID_ERROR, PARAMS_STRING[i], NEW_LINE);
				printf("%s %d%s", IN_LINE, lineCount, NEW_LINE);
				returnValue = FALSE;
			}
		}
	}

	return returnValue;
}

/**
 * Get students input from user
 */
void getStudentsInputFromUser()
{
	char inputVal[MAX_INPUT_LINE_LENGTH];
	int lineCount = 0;
	printf("%s%s", ASK_INPUT, NEW_LINE);
	if (fgets(inputVal, MAX_INPUT_LINE_LENGTH, stdin) == NULL)
	{
		strcpy(inputVal, NEW_LINE);
	}
	while (strcmp(inputVal, STOP) != TRUE)
	{
		Student newStudent;
		if (inputLineToStudent(&newStudent, inputVal, lineCount) == TRUE)
		{
			allStudents[studentsCount] = newStudent;
			studentsCount++;
		}
		lineCount++;
		printf("%s%s", ASK_INPUT, NEW_LINE);
		if (fgets(inputVal, MAX_INPUT_LINE_LENGTH, stdin) == NULL)
		{
			strcpy(inputVal, NEW_LINE);
		}
	}
}

/**
 * Merge between 2 sub arrays of students
 * @param firstStart    First sub array start index
 * @param firstEnd      First sub array end index
 * @param secondStart   Second sub array start index
 * @param secondEnd     Second sub array end index
 */
void mergeTwoStudentsSubArrays(int firstStart, int firstEnd, int secondStart, int secondEnd)
{
	Student helpArray[MAX_STUDENTS];
	int firstArrayIndex = firstStart;
	int secondArrayIndex = secondStart;
	int helpArrayIndex = 0;

	while (firstArrayIndex <= firstEnd && secondArrayIndex <= secondEnd)
	{
		if (allStudents[firstArrayIndex].grade <= allStudents[secondArrayIndex].grade)
		{
			helpArray[helpArrayIndex] = allStudents[firstArrayIndex];
			firstArrayIndex++;
		}
		else
		{
			helpArray[helpArrayIndex] = allStudents[secondArrayIndex];
			secondArrayIndex++;
		}
		helpArrayIndex++;
	}

	while (firstArrayIndex <= firstEnd)
	{
		helpArray[helpArrayIndex] = allStudents[firstArrayIndex];
		firstArrayIndex++;
		helpArrayIndex++;
	}

	while (secondArrayIndex <= secondEnd)
	{
		helpArray[helpArrayIndex] = allStudents[secondArrayIndex];
		secondArrayIndex++;
		helpArrayIndex++;
	}

	for (int i = 0; i < helpArrayIndex; i++)
	{
		allStudents[firstStart + i] = helpArray[i];
	}
}

/**
 * Merge sort students by their grades
 * @param lowIndex      Low index
 * @param highIndex     High index
 */
void mergeSortStudentsByGrades(int lowIndex, int highIndex)
{
	if (lowIndex >= highIndex)
	{
		return;
	}
	int middleIndex = (lowIndex + highIndex) / 2;
	mergeSortStudentsByGrades(lowIndex, middleIndex);
	mergeSortStudentsByGrades(middleIndex + 1, highIndex);
	mergeTwoStudentsSubArrays(lowIndex, middleIndex,
		middleIndex + 1, highIndex);
}

/**
 * Swap two students values
 * @param student1      First student pointer
 * @param student2      Second student pointer
 */
void swapStudents(Student* student1, Student* student2)
{
	Student helpStr = *student1;
	*student1 = *student2;
	*student2 = helpStr;
}

/**
 * Get divider for GetStudentsAndQuickSortByNames sort
 * @param lowIndex      Low index
 * @param highIndex     High index
 * @return              Divider
 */
int getDivider(int lowIndex, int highIndex)
{
	char* pivot = allStudents[highIndex].name;
	int smallestElementIndex = lowIndex - 1;
	for (int i = lowIndex; i <= highIndex; i++)
	{
		if (pivot > allStudents[i].name)
		{
			smallestElementIndex++;
			swapStudents(&allStudents[smallestElementIndex], &allStudents[i]);
		}
	}
	swapStudents(&allStudents[smallestElementIndex], &allStudents[highIndex]);
	return smallestElementIndex + 1;
}

/**
 * Quick sort students by their name
 * @param lowIndex      Low index
 * @param highIndex     High index
 */
void quickSortStudents(int lowIndex, int highIndex)
{
	if (highIndex > lowIndex)
	{
		int divider = getDivider(lowIndex, highIndex);
		quickSortStudents(lowIndex, divider - 1);
		quickSortStudents(divider + 1, highIndex);
	}
}

/**
 * Main function
 * @param argc      Arguments count
 * @param argv      Arguments array
 * @return          0 if worked, 1 otherwise
 */
int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("%s%s", USAGE_ERROR, NEW_LINE);
		return FALSE;
	}
	else
	{
		if (strcmp(argv[1], BEST) == TRUE)
		{
			getStudentsInputFromUser();
			if (studentsCount > 0)
			{
				printBestStudent();
			}
		}
		else if (strcmp(argv[1], MERGE) == TRUE)
		{
			getStudentsInputFromUser();
			if (studentsCount > 0)
			{
				mergeSortStudentsByGrades(0, studentsCount - 1);
				printAllStudents();
			}
		}
		else if (strcmp(argv[1], QUICK) == TRUE)
		{
			getStudentsInputFromUser();
			if (studentsCount > 0)
			{
				quickSortStudents(0, studentsCount - 1);
				printAllStudents();
			}
		}
		else
		{
			printf("%s%s", USAGE_ERROR, NEW_LINE);
			return FALSE;
		}
	}
	return TRUE;
}

