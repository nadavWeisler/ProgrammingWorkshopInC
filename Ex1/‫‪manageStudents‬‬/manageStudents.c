#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LINE_LENGTH 60
#define MAX_STUDENTS 5500

#define ID_LENGTH 10
#define FIELD_SIZE 43

#define TRUE 0
#define FALSE 1

#define ID_PARAM 0
#define NAME_PARAM 1
#define GRADE_PARAM 2
#define AGE_PARAM 3
#define COUNTRY_PARAM 4
#define CITY_PARAM 5

#define COMMA ','
#define EMPTY_CHAR '\0'
#define HYPHEN '-'
#define NEW_LINE '\n'

#define STOP 'q'
#define START_ERROR_MSG "ERROR:"
#define INVALID_ERROR "Invalid bestValue for"
#define IN_LINE "in line"
#define INVALID_PARAM_COUNT "invalid param count"
#define ASK_INPUT "‫‪Enter‬‬ ‫‪student‬‬ ‫‪info.‬‬ ‫‪To‬‬ ‫‪exit‬‬ ‫‪press‬‬ ‫‪q,‬‬ ‫‪then‬‬ ‫‪enter‬‬"
#define BEST_STUDENT_TEXT "‫‪best‬‬ ‫‪student‬‬ ‫‪info‬‬ ‫‪is:‬‬"
#define USAGE_ERROR "USAGE: You should write best, merge or quick, come on :("

#define BEST "best"
#define BEST_LENGTH 5
#define MERGE "merge"
#define MERGE_LENGTH 6
#define QUICK "quick"
#define QUICK_LENGTH 6

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

char PARAMS_STRING[6][7] = {"Id", "Name", "Grade", "Age", "Country", "City"};

int studentsCount = 0;
Student allStudents[MAX_STUDENTS];


/**
 * Generate input error messages
 * @param param         Params
 * @param line          Line
 * @param msg           Error msg
 */
void generateInputError(char *param, int line, char *msg)
{
    snprintf(msg, MAX_INPUT_LINE_LENGTH, "%s %s %s %s %d\n",
             START_ERROR_MSG, INVALID_ERROR, param, IN_LINE, line);
}

/**
 * Validate that String is int
 * @param param             String - char array
 * @param length            Param length
 * @return                  0 if int, 1 otherwise
 */
int stringIsInt(char *param, int length)
{
    for (int i = 0; i < length; i++)
    {
        if (param[i] == EMPTY_CHAR)
        {
            break;
        }
        else
        {
            if (!isdigit(param[i]))
            {
                return FALSE;
            }
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
int validateNumber(char *param, int minValue, int maxValue)
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
int validateString(char *param, int length)
{
    for (int i = 0; i < length; i++)
    {
        if (param[i] == EMPTY_CHAR)
        {
            break;
        }
        else if (!isalpha(param[i]) && param[i] == HYPHEN)
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
int getStringLength(char *str, int maxSize)
{
    int result = 0;
    for (int i = 0; i < maxSize; i++)
    {
        if (str[i] == EMPTY_CHAR)
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
int validateId(char *param)
{
    if (getStringLength(param, FIELD_SIZE) != 10)
    {
        return FALSE;
    }
    else if (stringIsInt(param, FIELD_SIZE) == FALSE)
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
int validateName(char *param)
{
    return validateString(param, FIELD_SIZE);
}


/**
 * Validate age param
 * @param param     Param to check
 * @return           Return 0 if true, 1 otherwise
 */
int validateAge(char *param)
{
    return validateNumber(param, 18, 120);
}

/**
 * Validate grade param
 * @param param     Param to check
 * @return           Return 0 if true, 1 otherwise
 */
int validateGrade(char *param)
{
    return validateNumber(param, 0, 100);
}

/**
 * Validate city param
 * @param param     Param to check
 * @return           Return 0 if true, 1 otherwise
 */
int validateCity(char *param)
{
    return validateString(param, FIELD_SIZE);
}

/**
 * Validate country param
 * @param param     Param to check
 * @return           Return 0 if true, 1 otherwise
 */
int validateCountry(char *param)
{
    return validateString(param, FIELD_SIZE);
}


/**
 * Validate param
 * @param param        Param to check
 * @param paramType    Param type
 * @return             Return 0 if true, 1 otherwise
 */
int validateParam(char *param, int paramType)
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
void resetString(char *str, int length)
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
void addParamToStudent(Student *student, char *paramVal, int paramType)
{
    int intParamVal = 0;
    switch (paramType)
    {
        case ID_PARAM:
            for (int i = 0; i < ID_LENGTH; i++)
            {
                student->id[i] = paramVal[i];
            }
            student->id[ID_LENGTH] = EMPTY_CHAR;
            break;
        case NAME_PARAM:
            for (int i = 0; i < FIELD_SIZE; i++)
            {
                student->name[i] = paramVal[i];
                if (paramVal[i] == EMPTY_CHAR)
                {
                    break;
                }
            }
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
            for (int i = 0; i < FIELD_SIZE; i++)
            {
                student->country[i] = paramVal[i];
                if (paramVal[i] == EMPTY_CHAR)
                {
                    break;
                }
            }
            break;
        case CITY_PARAM:
            for (int i = 0; i < FIELD_SIZE; i++)
            {
                student->city[i] = paramVal[i];
                if (paramVal[i] == EMPTY_CHAR)
                {
                    break;
                }
            }
            break;
    }
}

/**
 * Get best student index from allstudents array
 * @return      Best student index
 */
int getBestStudent()
{
    float bestValue = -1.0;
    int index = -1;
    for (int i = 0; i < studentsCount; i++)
    {
        float grade = (float) allStudents[i].grade;
        float age = (float) allStudents[i].age;
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
    if (index > -1)
    {
        char studentText[MAX_INPUT_LINE_LENGTH];
        sprintf(studentText, "%s %s,%s,%d,%d,%s,%s", BEST_STUDENT_TEXT,
                allStudents[index].id, allStudents[index].name,
                allStudents[index].grade, allStudents[index].age,
                allStudents[index].country, allStudents[index].city);
        fputs(studentText, stdout);
    }
}


/**
 * Update fields to existing student
 * @param paramCount        Param count (0-5)
 * @param field             Field value
 * @param newStudent        Student pointer
 * @return                  0 if worked, 1 otherwise
 */
int updateFieldsInStudent(int paramCount, char *field, Student *newStudent)
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
int inputLineToStudent(Student *newStudent, char *errorMsg, char *inputLine, int lineLength, int lineCount)
{
    int paramCount = 0;
    int returnValue = TRUE;
    char field[FIELD_SIZE];
    int startField = 0;
    int error_break = FALSE;
    for (int i = 0; i < lineLength; i++)
    {
        if (paramCount > CITY_PARAM)
        {
            break;
        }
        else if (inputLine[i] == NEW_LINE)
        {
            continue;
        }
        else if (inputLine[i] == COMMA)
        {
            if (updateFieldsInStudent(paramCount, field, newStudent) == TRUE)
            {
                resetString(field, i - (startField));
                (startField) = i + 1;
                paramCount++;
            }
            else
            {
                error_break = TRUE;
                break;
            }
        }
        else
        {
            field[i - startField] = inputLine[i];
        }
    }
    if (error_break == FALSE)
    {
        if (paramCount != CITY_PARAM)
        {
            sprintf(errorMsg, "%s %s %s %d", START_ERROR_MSG, INVALID_PARAM_COUNT, IN_LINE, lineCount);
            returnValue = FALSE;

        }
        else
        {
            if (updateFieldsInStudent(paramCount, field, newStudent) == TRUE)
            {
            }
            else
            {
                generateInputError(PARAMS_STRING[paramCount], lineCount, errorMsg);
                returnValue = FALSE;
            }
        }
    }
    else
    {
        generateInputError(PARAMS_STRING[paramCount], lineCount, errorMsg);
        returnValue = FALSE;
    }

    return returnValue;
}

/**
 * Check if stop
 * @param input     Input to check
 * @return           0 if stop, 1 otherwise
 */
int ifStop(char *input)
{
    if (input[0] == NEW_LINE)
    {
        return FALSE;
    }
    if (input[0] == STOP && input[1] == NEW_LINE)
    {
        return TRUE;
    }
    return FALSE;
}

/**
 * Check if input is equal to str
 * @param input     String
 * @param str       String
 * @param length    Length
 * @return          0 if equal, 1 otherwise
 */
int ifEqual(char *input, char *str, int length)
{
    for (int i = 0; i < length; i++)
    {
        if ((str[i] == EMPTY_CHAR || str[i] == NEW_LINE) && (input[i] == EMPTY_CHAR || input[i] == NEW_LINE))
        {
            return TRUE;
        }
        else if (str[i] != input[i])
        {
            return FALSE;
        }
    }
    return FALSE;
}

/**
 * Get students input from user
 */
void getStudentsInputFromUser()
{
    char inputVal[MAX_INPUT_LINE_LENGTH];
    int lineCount = 0;
    printf("%s\n", ASK_INPUT);
    fgets(inputVal, MAX_INPUT_LINE_LENGTH, stdin);
    while (ifStop(inputVal) == FALSE)
    {
        Student newStudent;
        char errorMsg[MAX_INPUT_LINE_LENGTH];
        if (inputLineToStudent(&newStudent, errorMsg, inputVal, MAX_INPUT_LINE_LENGTH, lineCount) == TRUE)
        {
            allStudents[studentsCount] = newStudent;
            studentsCount++;
        }
        else
        {
            fputs(errorMsg, stdout);
        }
        lineCount++;
        printf("%s\n", ASK_INPUT);
        fgets(inputVal, MAX_INPUT_LINE_LENGTH, stdin);
    }
}

/**
 * Get students from user and print the best one
 */
void getStudentsAndPrintBest()
{
    getStudentsInputFromUser();
    printBestStudent();
}


/**
 * Merge between 2 sub arrays of students
 * @param firstStart    First sub array start index
 * @param firstEnd      First sub array end index
 * @param secondStart   Second sub array start index
 * @param secondEnd     Second sub array end index
 */
void mergeTwoStudentsSubArray(int firstStart, int firstEnd, int secondStart, int secondEnd)
{
    Student helpArray[studentsCount];
    int firstArrayIndex = firstStart;
    int secondArrayIndex = secondStart;
    int helpArrayIndex = 0;

    while (firstArrayIndex <= firstEnd && secondArrayIndex <= secondEnd)
    {
        if (allStudents[firstArrayIndex].grade >= allStudents[secondArrayIndex].grade)
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

    for (int i = 0; i <= helpArrayIndex; i++)
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
    mergeTwoStudentsSubArray(lowIndex, middleIndex, middleIndex + 1, highIndex);
}

/**
 * Swap two students values
 * @param student1      First student pointer
 * @param student2      Second student pointer
 */
void swapStudents(Student *student1, Student *student2)
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
    char *pivot = allStudents[highIndex].name;
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
 * Get students from users and merge sort them by their grades
 */
void getStudentsAndMergeSortByGrades()
{
    getStudentsInputFromUser();
    mergeSortStudentsByGrades(0, studentsCount);
}


/**
 * Get students from users and quick sort them by their names
 */
void getStudentsAndQuickSortByNames()
{
    getStudentsInputFromUser();
    quickSortStudents(0, studentsCount - 1);
}

/**
 * Main function
 * @param argc      Arguments count
 * @param argv      Arguments array
 * @return          0 if worked, 1 otherwise
 */
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("%s\n", USAGE_ERROR);
        return FALSE;
    }
    else
    {
        if (ifEqual(argv[1], BEST, BEST_LENGTH))
        {
            getStudentsAndPrintBest();
        }
        else if (ifEqual(argv[1], MERGE, MERGE_LENGTH))
        {
            getStudentsAndMergeSortByGrades();
        }
        else if (ifEqual(argv[1], QUICK, QUICK_LENGTH))
        {
            getStudentsAndQuickSortByNames();
        }
        else
        {
            printf("%s\n", USAGE_ERROR);
            return FALSE;
        }
    }
    return TRUE;
}

