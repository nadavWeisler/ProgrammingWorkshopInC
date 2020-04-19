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

struct Student {
    char id[ID_LENGTH + 1];
    char name[FIELD_SIZE];
    int grade;
    int age;
    char country[FIELD_SIZE];
    char city[FIELD_SIZE];
};

void GetStudentsInputFromUser();

typedef struct Student Student;

char PARAMS_STRING[6][7] = {"Id", "Name", "Grade", "Age", "Country", "City"};

int studentsCount = 0;
Student allStudents[MAX_STUDENTS];

/// Print error messages
/// \param param        Params
/// \param msg          Error msg
/// \param line         Line
void GenerateInputError(char *param, int line, char *msg) {
    snprintf(msg, MAX_INPUT_LINE_LENGTH, "%s %s %s %s %d\n",
             START_ERROR_MSG, INVALID_ERROR, param, IN_LINE, line);
}

/// Validate that String is int
/// \param param        String - char array
/// \param length       Param length
/// \return             0 if int, 1 otherwise
int StringIsInt(char *param, int length) {
    for (int i = 0; i < length; i++) {
        if (param[i] == EMPTY_CHAR) {
            break;
        } else {
            if (!isdigit(param[i])) {
                return FALSE;
            }
        }
    }
    return TRUE;
}

/// Test numbers by check if it is number and between minValue and maxValue
/// \param param        Param to check
/// \param minValue     Minimal Value
/// \param maxValue     Maximum Value
/// \return             Return 0 if true, 1 otherwise
int TestNumber(char *param, int minValue, int maxValue) {
    if (StringIsInt(param, FIELD_SIZE) == FALSE) {
        return FALSE;
    }
    int intParam = atoi(param);
    if (intParam > maxValue || intParam < minValue) {
        return FALSE;
    }
    return TRUE;
}

/// Test string bestValue
/// \param param        Param to validate
/// \param length       Length to iterate
/// \return             0 is True, 1 otherwise
int TestString(char *param, int length) {
    for (int i = 0; i < length; i++) {
        if (param[i] == EMPTY_CHAR) {
            break;
        } else if (!isalpha(param[i]) && param[i] == HYPHEN) {
            return FALSE;
        }
    }
    return TRUE;
}

/// Get string length
/// \param str          String reference
/// \param maxSize      Max string size
/// \return             String length
int GetStringLength(char *str, int maxSize) {
    int result = 0;
    for (int i = 0; i < maxSize; i++) {
        if (str[i] == EMPTY_CHAR) {
            break;
        } else {
            result++;
        }
    }
    return result;
}

/// Test ID bestValue
/// \param param        Param to check
/// \return             Return 0 if true, 1 otherwise
int TestId(char *param) {
    if (GetStringLength(param, FIELD_SIZE) != 10) {
        return FALSE;
    } else if (StringIsInt(param, FIELD_SIZE) == FALSE) {
        return FALSE;
    }
    return TRUE;
}

/// Test name param
/// \param param        Param to check
/// \return             Return 0 if true, 1 otherwise
int TestName(char *param) {
    return TestString(param, FIELD_SIZE);
}

/// Test age param
/// \param param        Param to check
/// \return             Return 0 if true, 1 otherwise
int TestAge(char *param) {
    return TestNumber(param, 18, 120);
}

/// Test grade param
/// \param param        Param to check
/// \return             Return 0 if true, 1 otherwise
int TestGrade(char *param) {
    return TestNumber(param, 0, 100);
}

/// Test city param
/// \param param        Param to check
/// \return             Return 0 if true, 1 otherwise
int TestCity(char *param) {
    return TestString(param, FIELD_SIZE);
}

/// Test country param
/// \param param        Param to check
/// \return             Return 0 if true, 1 otherwise
int TestCountry(char *param) {
    return TestString(param, FIELD_SIZE);
}

/// Test param
/// \param param        Param to check
/// \param paramType    Param type
/// \return             Return 0 if true, 1 otherwise
int TestParam(char *param, int paramType) {
    switch (paramType) {
        case ID_PARAM:
            return TestId(param);
        case NAME_PARAM:
            return TestName(param);
        case AGE_PARAM:
            return TestAge(param);
        case GRADE_PARAM:
            return TestGrade(param);
        case COUNTRY_PARAM:
            return TestCountry(param);
        case CITY_PARAM:
            return TestCity(param);
        default:
            return FALSE;
    }
}

/// Rest string array
/// \param str          Char array
/// \param length       Array length
void ResetString(char *str, int length) {
    for (int i = 0; i < length; i++) {
        str[i] = '\0';
    }
}

/// Add param to student object
/// \param student      Student pointer
/// \param paramVal     Param bestValue
/// \param paramType    Param Type
void AddParamToStudent(Student *student, char *paramVal, int paramType) {
    int intParamVal = 0;
    switch (paramType) {
        case ID_PARAM:
            for (int i = 0; i < ID_LENGTH; i++) {
                student->id[i] = paramVal[i];
            }
            student->id[ID_LENGTH] = EMPTY_CHAR;
            break;
        case NAME_PARAM:
            for (int i = 0; i < FIELD_SIZE; i++) {
                student->name[i] = paramVal[i];
                if (paramVal[i] == EMPTY_CHAR) {
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
            for (int i = 0; i < FIELD_SIZE; i++) {
                student->country[i] = paramVal[i];
                if (paramVal[i] == EMPTY_CHAR) {
                    break;
                }
            }
            break;
        case CITY_PARAM:
            for (int i = 0; i < FIELD_SIZE; i++) {
                student->city[i] = paramVal[i];
                if (paramVal[i] == EMPTY_CHAR) {
                    break;
                }
            }
            break;
    }
}

int GetBestStudent() {
    float bestValue = -1.0;
    int index = -1;
    for (int i = 0; i < studentsCount; i++) {
        float grade = (float) allStudents[i].grade;
        float age = (float) allStudents[i].age;
        if ((grade / age) > bestValue) {
            bestValue = grade / age;
            index = i;
        }
    }
    return index;
}

/// Print best student
void printBestStudent() {
    int index = GetBestStudent();
    if (index > -1) {
        char studentText[MAX_INPUT_LINE_LENGTH];
        sprintf(studentText, "%s %s,%s,%d,%d,%s,%s", BEST_STUDENT_TEXT,
                allStudents[index].id, allStudents[index].name,
                allStudents[index].grade, allStudents[index].age,
                allStudents[index].country, allStudents[index].city);
        fputs(studentText, stdout);
    }
}

/// Update fields to existing student
/// \param paramCount   Param count (0-5)
/// \param field        Field bestValue
/// \param newStudent   Student pointer
/// \return             0 if worked, 1 otherwise
int updateFieldsInStudent(int paramCount, char *field, Student *newStudent) {
    if (TestParam(field, paramCount) == TRUE) {
        AddParamToStudent(newStudent, field, paramCount);
        return TRUE;
    } else {
        return FALSE;
    }
}

/// Generate input line to student
/// \param inputLine        Input line (char array)
/// \param lineLength       Input line length
/// \return                 Student object
int inputLineToStudent(Student *newStudent, char *errorMsg, char *inputLine, int lineLength, int lineCount) {
    int paramCount = 0;
    int returnValue = TRUE;
    char field[FIELD_SIZE];
    int startField = 0;
    int error_break = FALSE;
    for (int i = 0; i < lineLength; i++) {
        if (paramCount > CITY_PARAM) {
            break;
        } else if (inputLine[i] == NEW_LINE) {
            continue;
        } else if (inputLine[i] == COMMA) {
            if (updateFieldsInStudent(paramCount, field, newStudent) == TRUE) {
                ResetString(field, i - (startField));
                (startField) = i + 1;
                paramCount++;
            } else {
                error_break = TRUE;
                break;
            }
        } else {
            field[i - startField] = inputLine[i];
        }
    }
    if (error_break == FALSE) {
        if (paramCount != CITY_PARAM) {
            sprintf(errorMsg, "%s %s %s %d\n", START_ERROR_MSG, INVALID_PARAM_COUNT, IN_LINE, lineCount);
            returnValue = FALSE;

        } else {
            if (updateFieldsInStudent(paramCount, field, newStudent) == TRUE) {
            } else {
                GenerateInputError(PARAMS_STRING[paramCount], lineCount, errorMsg);
                returnValue = FALSE;
            }
        }
    } else {
        GenerateInputError(PARAMS_STRING[paramCount], lineCount, errorMsg);
        returnValue = FALSE;
    }

    return returnValue;
}

/// Check if stop
/// \param input        Input to check
/// \return             0 if stop, 1 otherwise
int IfStop(char *input) {
    if (input[0] == NEW_LINE) {
        return FALSE;
    }
    if (input[0] == STOP && input[1] == NEW_LINE) {
        return TRUE;
    }
    return FALSE;
}

/// Get best student
/// \return
int best() {
    GetStudentsInputFromUser();
    printBestStudent();
    return 0;
}

/// Get students input from user
void GetStudentsInputFromUser() {
    char inputVal[MAX_INPUT_LINE_LENGTH];
    int lineCount = 0;
    printf("%s\n", ASK_INPUT);
    fgets(inputVal, MAX_INPUT_LINE_LENGTH, stdin);
    while (IfStop(inputVal) == FALSE) {
        Student newStudent;
        char errorMsg[MAX_INPUT_LINE_LENGTH];
        if (inputLineToStudent(&newStudent, errorMsg, inputVal, MAX_INPUT_LINE_LENGTH, lineCount) == TRUE) {
            allStudents[studentsCount] = newStudent;
            studentsCount++;
        } else {
            fputs(errorMsg, stdout);
        }
        lineCount++;
        printf("%s\n", ASK_INPUT);
        fgets(inputVal, MAX_INPUT_LINE_LENGTH, stdin);
    }
}

/// Merge between 2 sub arrays of students
/// \param firstStart       First sub array start index
/// \param firstEnd         First sub array end index
/// \param secondStart      Second sub array start index
/// \param secondEnd        Second sub array end index
void MergeFunction(int firstStart, int firstEnd, int secondStart, int secondEnd){
    Student helpArray[studentsCount];
    int firstArrayIndex = firstStart;
    int secondArrayIndex = secondStart;
    int helpArrayIndex = 0;

    while(firstArrayIndex <= firstEnd && secondArrayIndex <= secondEnd){
        if(allStudents[firstArrayIndex].grade >= allStudents[secondArrayIndex].grade){
            helpArray[helpArrayIndex] = allStudents[firstArrayIndex];
            firstArrayIndex++;
        } else {
            helpArray[helpArrayIndex] = allStudents[secondArrayIndex];
            secondArrayIndex++;
        }
        helpArrayIndex++;
    }

    while(firstArrayIndex <= firstEnd) {
        helpArray[helpArrayIndex] = allStudents[firstArrayIndex];
        firstArrayIndex++;
        helpArrayIndex++;
    }

    while(secondArrayIndex <= secondEnd) {
        helpArray[helpArrayIndex] = allStudents[secondArrayIndex];
        secondArrayIndex++;
        helpArrayIndex++;
    }

    for(int i = 0; i <= helpArrayIndex ;i++){
        allStudents[firstStart + i] = helpArray[i];
    }
}

/// Merge sort students by their grades
/// \param lowIndex         Low index
/// \param highIndex        High index
void MergeSortStudents(int lowIndex, int highIndex) {
    if(lowIndex >= highIndex) {
        return;
    }
    int middleIndex = (lowIndex + highIndex) / 2;
    MergeSortStudents(lowIndex, middleIndex);
    MergeSortStudents(middleIndex + 1, highIndex);
    MergeFunction(lowIndex, middleIndex, middleIndex + 1, highIndex);
}

/// Swap two students values
/// \param student1         First student pointer
/// \param student2         Second student pointer
void SwapStudents(Student *student1, Student *student2) {
    Student helpStr = *student1;
    *student1 = *student2;
    *student2 = helpStr;
}

/// Get divider for quick sort
/// \param lowIndex         Low index
/// \param highIndex        High index
/// \return
int GetDivider(int lowIndex, int highIndex) {
    char *pivot = allStudents[highIndex].name;
    int smallestElementIndex = lowIndex - 1;
    for (int i = lowIndex; i <= highIndex; i++) {
        if (pivot > allStudents[i].name) {
            smallestElementIndex++;
            SwapStudents(&allStudents[smallestElementIndex], &allStudents[i]);
        }
    }
    SwapStudents(&allStudents[smallestElementIndex], &allStudents[highIndex]);
    return smallestElementIndex + 1;
}

/// Quick sort students by their name
/// \param lowIndex         Low index
/// \param highIndex        High index
void QuickSortStudents(int lowIndex, int highIndex) {
    if (highIndex > lowIndex) {
        int divider = GetDivider(lowIndex, highIndex);
        QuickSortStudents(lowIndex, divider - 1);
        QuickSortStudents(divider + 1, highIndex);
    }
}

///
/// \return
int merge() {
    GetStudentsInputFromUser();
    MergeSortStudents(0, studentsCount);
    return 0;
}

///
int quick() {
    GetStudentsInputFromUser();
    QuickSortStudents(0, studentsCount - 1);
    return 0;
}

