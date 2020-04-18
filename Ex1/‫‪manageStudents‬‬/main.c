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

struct Student {
    char id[ID_LENGTH + 1];
    char name[FIELD_SIZE];
    int grade;
    int age;
    char country[FIELD_SIZE];
    char city[FIELD_SIZE];
    int realStudent;
};
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

/// Print student
/// \param student      Student object
void printStudent(Student student) {
    printf("Id: %s\n", student.id);
    printf("Name: %s\n", student.name);
    printf("Grade: %d\n", student.grade);
    printf("Age: %d\n", student.age);
    printf("Country: %s\n", student.country);
    printf("City: %s\n", student.city);
    printf("Real: %d\n", student.realStudent);
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

int GetBestStudent() {
    float bestValue = -1.0;
    int index = -1;
    for (int i = 0; i < studentsCount; i++) {
        float grade = (float) allStudents[studentsCount].grade;
        float age = (float) allStudents[studentsCount].age;
        if ((grade / age) > bestValue) {
            bestValue = grade / age;
            index = i;
        }
    }
    return index;
}

/// Main function
/// \return
int main() {
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
    int index = GetBestStudent();
    if(index != -1){
        printStudent(allStudents[index]);
    }
    return 0;
}