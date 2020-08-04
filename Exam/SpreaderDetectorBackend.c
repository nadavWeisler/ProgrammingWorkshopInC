#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SpreaderDetectorParams.h"

#define OUTPUT_FILE "SpreaderDetectorAnalysis.out"
#define OPEN_FILE_PERMISSION "r"
#define WRITE_FILE_PERMISSION "w"
#define NEW_LINE_CHAR '\n'
#define BACKSLASH_R '\r'
#define LINE_LENGTH 1024
#define TRUE 0
#define FALSE 1
#define CONVERSION_BASE 10
#define BAD_FORMAT_ERROR "Usage: ./SpreaderDetectorBackend %s %s\n"
#define ERROR_IN_INPUT_FILE "Error in input files.\n"
#define ERROR_IN_OUTPUT_FILE "Error in output file.\n”"
#define POINT '.'
#define SUCCESS 0
#define ERROR 1
#define LIBRARY_ERROR 2
#define INFECTS_ID 0
#define INFECTED_ID 1
#define DISTANCE 2
#define TIME 3
#define ID 1
#define NAME 0
#define PEOPLE_ARGS_COUNT 3
#define MEETINGS_ARGS_COUNT 4

/**
 * @brief Struct that define a peron
 */
typedef struct person
{
    char *name;
    unsigned long id;
    float chance;
} Person;

/**
 * @brief Person BST struct
 */
typedef struct personNode
{
    Person *person;
    struct personNode *left;
    struct personNode *right;
} PersonNode;

/**
 * @brief Person pointers BST(binary search tree) tree
 */
PersonNode *peopleTree;

/**
 * @brief Person pointers array
 */
Person **peoples;

/**
 * @brief People count
 */
int peopleCount;

/**
 * @brief           Free root
 * @param root      Person node
 */
void freeTree(PersonNode *root);

/**
 * @brief           Find person ib PersonNode BST
 * @param node      BST node
 * @param id        Person id (unsigned long)
 * @return          Person pointer
 */
Person *findPerson(PersonNode *node, unsigned long id);

/**
 * @brief           Create new node from person
 * @param person    Person pointer
 * @return          PersonNode
 */
PersonNode *createNewNode(Person *person);

/**
 * @brief           Insert person to PersonNode BST
 * @param node      PersonNode node
 * @param person    Person pointer
 * @return          PersonNode root
 */
PersonNode *insert(PersonNode *node, Person *person);

/**
 * @brief           Remove new line and space from string
 * @param str       String, (char array)
 */
void removeNewLineChar(char *str);

/**
 * @brief Free people collection
 */
void freePeoples();

/**
 * @brief               Infection probability formula
 * @param distance      Distance (float)
 * @param time          Time (float)
 * @return              Infection probability (float)
 */
float coronaChance(float distance, float time);

/**
 * @brief           Sort people collection by quick sort algorithm
 * @param lowIndex       Low index
 * @param highIndex      High index
 */
void quickSort(int lowIndex, int highIndex);

/**
 * @brief       Validate that str represent long
 * @param str   String
 * @return      TRUE if represent long, FALSE otherwise
 */
int validateLong(char *str);

/**
 * @brief           Calculate partition for quick sort algorithm
 * @param lowIndex       Low index
 * @param highIndex      High index
 * @return
 */
int partition(int lowIndex, int highIndex);

/**
 * @brief           Update person value by file line
 * @param person    Person pointer
 * @param line      Line (string)
 */
int updatePersonFromLine(Person *person, char *line);

/**
 * @brief           Update person infection from line from meeting file
 * @param line      Line from meeting file (string)
 */
int updatePersonInfectedFromLine(char *line);

/**
 * @brief           Read people in file
 * @param file      People in file pointer
 * @return          TRUE is succeeded, FALSE otherwise
 */
int readPeopleFile(FILE *file);

/**
 * @brief        Validate that str represent float
 * @param str    String
 * @return       TRUE if represent long, FALSE otherwise
 */
int validateFloat(char *str);

/**
 * @brief Sort people (quicksort algorithm)
 */
void sortPeoples();

/**
 * @brief       Validate Id (If long and 9 letters)
 * @param str   String
 * @return      TRUE if represent long, FALSE otherwise
 */
int validateId(char *str);

/**
 * @brief           Read meetings file
 * @param file      File pointer
 * @return          TRUE is succeeded, FALSE otherwise
 */
int readMeetingsFile(FILE *file);

/**
 * @brief                   Write to output file
 * @param outputFilePath    Output file path
 */
int writeOutputFile(const char *outputFilePath);

/**
 * @brief               Infection probability formula
 * @param distance      Distance (float)
 * @param time          Time (float)
 * @return              Infection probability (float)
 */
float coronaChance(float distance, float time)
{
    return (time * MIN_DISTANCE) / (distance * MAX_TIME);
}

/**
 * @brief           Update person value by file line
 * @param person    Person pointer
 * @param line      Line (string)
 */
int updatePersonFromLine(Person *person, char *line)
{
    char *splitLine = strtok(line, " ");
    int count = 0;
    char **literal = NULL;
    //Iterate split values by empty space
    while (splitLine != NULL)
    {
        //Too much properties
        if (count == PEOPLE_ARGS_COUNT)
        {
            return ERROR;
        }
        if (count == NAME)
        {
            person->name = malloc(strlen(splitLine) + 1);
            if (person->name == NULL)
            {
                return LIBRARY_ERROR;
            }
            strcpy(person->name, splitLine);
        }
        else if (count == ID)
        {
            person->id = strtol(splitLine, literal, CONVERSION_BASE);
            if (literal != NULL && splitLine == *literal && person->id == 0)
            {
                return LIBRARY_ERROR;
            }
        }
        count++;
        splitLine = strtok(NULL, " ");
    }
    //Not all the properties set
    if (count < PEOPLE_ARGS_COUNT)
    {
        return ERROR;
    }
    //Set person infection chance to 0
    person->chance = 0;
    return SUCCESS;
}

/**
 * @brief       Validate that str represent long
 * @param str   String
 * @return      TRUE if represent long, FALSE otherwise
 */
int validateLong(char *str)
{
    removeNewLineChar(str);
    for (size_t i = 0; i < strlen(str); i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * @brief       Validate Id (If long and 9 letters)
 * @param str   String
 * @return      TRUE if represent long, FALSE otherwise
 */
int validateId(char *str)
{
    removeNewLineChar(str);
    if (validateLong(str) == TRUE && strlen(str) == 9)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
 * @brief        Validate that str represent float
 * @param str    String
 * @return       TRUE if represent long, FALSE otherwise
 */
int validateFloat(char *str)
{
    removeNewLineChar(str);
    int sawPoint = FALSE;
    for (size_t i = 0; i < strlen(str); i++)
    {
        if ('0' > str[i] || str[i] > '9')
        {
            if ((str[i] == POINT) && (sawPoint == FALSE))
            {
                sawPoint = TRUE;
            }
            else
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

/**
 * @brief           Update person infection from line from meeting file
 * @param line      Line from meeting file (string)
 */
int updatePersonInfectedFromLine(char *line)
{
    char *splitLine = strtok(line, " ");
    int count = 0;
    char **literal = NULL;
    Person *infects;
    Person *infected;
    float distance;
    float time;
    unsigned long currentId;
    //Iterate each value on split line by empty space
    while (splitLine != NULL)
    {
        //Too much arguments
        if (count == MEETINGS_ARGS_COUNT)
        {
            return ERROR;
        }
        if (count == INFECTS_ID)
        {
            if (validateId(splitLine) == FALSE)
            {
                return ERROR;
            }
            currentId = strtol(splitLine, literal, CONVERSION_BASE);
            if (literal != NULL && currentId == 0 && splitLine == *literal)
            {
                return LIBRARY_ERROR;
            }
            infects = findPerson(peopleTree, currentId);
        }
        else if (count == INFECTED_ID)
        {
            if (validateId(splitLine) == FALSE)
            {
                return ERROR;
            }
            currentId = strtol(splitLine, literal, CONVERSION_BASE);
            if (literal != NULL && currentId == 0 && splitLine == *literal)
            {
                return LIBRARY_ERROR;
            }
            infected = findPerson(peopleTree, currentId);
        }
        else if (count == DISTANCE)
        {
            if (validateFloat(splitLine) == FALSE)
            {
                return ERROR;
            }
            distance = strtof(splitLine, literal);
            if (literal != NULL && distance == 0 && splitLine == *literal)
            {
                return LIBRARY_ERROR;
            }
        }
        else if (count == TIME)
        {
            if (validateFloat(splitLine) == FALSE)
            {
                return ERROR;
            }
            time = strtof(splitLine, literal);
            if (literal != NULL && time == 0 && splitLine == *literal)
            {
                return LIBRARY_ERROR;
            }
        }
        else
        {
            return ERROR;
        }
        count++;
        splitLine = strtok(NULL, " ");
    }

    //If there where under then 4 arguments
    if (count < MEETINGS_ARGS_COUNT)
    {
        return ERROR;
    }

    //Update infected infection chance
    infected->chance = coronaChance(distance, time);
    if (infects->chance != 0)
    {
        infected->chance *= infects->chance;
    }
    return SUCCESS;
}

/**
 * @brief           Read people in file
 * @param file      People in file pointer
 * @return          TRUE is succeeded, FALSE otherwise
 */
int readPeopleFile(FILE *file)
{
    char line[LINE_LENGTH];
    peoples = (Person **) malloc(0);
    if (peoples == NULL)
    {
        return LIBRARY_ERROR;
    }
    Person *newPerson;
    //Iterate file lines
    while (fgets(line, LINE_LENGTH, file))
    {
        if (line[0] == NEW_LINE_CHAR)
        {
            continue;
        }
        removeNewLineChar(line);
        //Create new Person
        newPerson = (Person *) malloc(sizeof(Person));
        if (newPerson == NULL)
        {
            return LIBRARY_ERROR;
        }
        //Update Person by people line
        int updateResult = updatePersonFromLine(newPerson, line);
        if (updateResult == SUCCESS)
        {
            //Add Person pointer to BST
            peopleTree = insert(peopleTree, newPerson);
            if (peopleTree == NULL)
            {
                return LIBRARY_ERROR;
            }
            peopleCount++;
            //Add Person pointer to array
            peoples = (Person **) realloc(peoples, peopleCount * sizeof(Person *));
            if (peoples == NULL)
            {
                return LIBRARY_ERROR;
            }
            peoples[peopleCount - 1] = newPerson;
        }
        //Update went wrong
        else
        {
            return updateResult;
        }
    }
    return SUCCESS;
}

/**
 * @brief           Remove new line and space from string
 * @param str       String, (char array)
 */
void removeNewLineChar(char *str)
{
    unsigned long strLen = strlen(str);
    if (str[strLen - 1] == NEW_LINE_CHAR || str[strLen - 1] == BACKSLASH_R)
    {
        str[strLen - 1] = '\0';
    }
}

/**
 * @brief           Read meetings file
 * @param file      File pointer
 * @return          TRUE is succeeded, FALSE otherwise
 */
int readMeetingsFile(FILE *file)
{
    char line[LINE_LENGTH];
    int firstLine = TRUE;
    unsigned long currentId;
    Person *currentPerson;
    char **literal = NULL;
    //Iterate file lines
    while (fgets(line, LINE_LENGTH, file))
    {
        removeNewLineChar(line);
        if (firstLine == TRUE)
        {
            if (validateId(line) == FALSE)
            {
                return ERROR;
            }
            currentId = strtol(line, literal, CONVERSION_BASE);
            if (literal != NULL && line == *literal && currentId == 0)
            {
                return LIBRARY_ERROR;
            }
            //Get Person pointer by his id
            currentPerson = findPerson(peopleTree, currentId);
            currentPerson->chance = 1;
            firstLine = FALSE;
        }
        else
        {
            //Empty line
            if (line[0] == NEW_LINE_CHAR)
            {
                continue;
            }
            //Update Person infection by meeting line
            int updateResult = updatePersonInfectedFromLine(line);
            //Update went wrong
            if (updateResult != SUCCESS)
            {
                return updateResult;
            }
        }
    }
    return SUCCESS;
}

/**
 * @brief                   Write to output file
 * @param outputFilePath    Output file path
 */
int writeOutputFile(const char *outputFilePath)
{
    FILE *outputFile = fopen(outputFilePath, WRITE_FILE_PERMISSION);
    if (outputFilePath == NULL)
    {
        return ERROR;
    }
    Person *currentPerson;
    char *currentMsg;
    //Iterate peoples and print each Person with the relevant message
    for (int i = 0; i < peopleCount; i++)
    {
        currentPerson = peoples[i];
        if (currentPerson->chance >= MEDICAL_SUPERVISION_THRESHOLD)
        {
            currentMsg = MEDICAL_SUPERVISION_THRESHOLD_MSG;
        }
        else if (currentPerson->chance >= REGULAR_QUARANTINE_THRESHOLD)
        {
            currentMsg = REGULAR_QUARANTINE_MSG;
        }
        else
        {
            currentMsg = CLEAN_MSG;
        }
        fprintf(outputFile, currentMsg, currentPerson->name, currentPerson->id);
    }
    fclose(outputFile);
    return SUCCESS;
}

/**
 * @brief Free people collection
 */
void freePeoples()
{
    if (peopleCount > 0)
    {
        for (int i = 0; i < peopleCount; i++)
        {
            //Free Person name
            free(peoples[i]->name);
            //Free Person
            free(peoples[i]);
        }
        //Free Person array
        free(peoples);
    }
    peoples = NULL;
}

/**
 * @brief           Calculate partition for quick sort algorithm
 * @param lowIndex       Low index
 * @param highIndex      High index
 * @return
 */
int partition(int lowIndex, int highIndex)
{
    //Set the Person at the high index as pivot
    Person *pivot = peoples[highIndex];
    //Define wall as the low index
    int wall = lowIndex;
    for (int i = lowIndex; i < highIndex; i++)
    {
        //If people in the index i chance is bigger than pivot chance
        if (peoples[i]->chance > pivot->chance)
        {
            //Swap wall Person and current Person, increase wall by one
            Person *temp = peoples[wall];
            peoples[wall] = peoples[i];
            peoples[i] = temp;
            wall++;
        }
    }
    //Set Person at high index to be the Person in wall index
    peoples[highIndex] = peoples[wall];
    //Set Person at wall index to be the pivot person
    peoples[wall] = pivot;
    return wall;
}

/**
 * @brief           Sort people collection by quick sort algorithm
 * @param lowIndex       Low index
 * @param highIndex      High index
 */
void quickSort(int lowIndex, int highIndex)
{
    if (lowIndex < highIndex)
    {
        //Calculate pivot index
        int pivotIndex = partition(lowIndex, highIndex);
        //Sort people array from low index to pivot index - 1
        quickSort(lowIndex, pivotIndex - 1);
        //Sort people array from pivotIndex + 1 to high index
        quickSort(pivotIndex + 1, highIndex);
    }
}

/**
 * @brief Sort people (quicksort algorithm)
 */
void sortPeoples()
{
    //Sort peoples array by quick-sort algorithm, from 0 to peopleCount -1
    quickSort(0, peopleCount - 1);
}

/**
 * @brief           Free root
 * @param root      Person node
 */
void freeTree(PersonNode *root)
{
    if (root != NULL)
    {
        //Free left son
        freeTree(root->left);
        //Free right son
        freeTree(root->right);
        //Free current root
        free(root);
    }
}

/**
 * @brief           Find person ib PersonNode BST
 * @param node      BST node
 * @param id        Person id (unsigned long)
 * @return          Person pointer
 */
Person *findPerson(PersonNode *node, unsigned long id)
{
    //Node is NULL
    if (node == NULL)
    {
        return NULL;
    }
        //Person found
    else if (node->person->id == id)
    {
        return node->person;
    }
        //Id is bigger than node person Id, try the right son
    else if (node->person->id < id)
    {
        return findPerson(node->right, id);
    }
        //Id is smaller than node person Id, try the small son
    else
    {
        return findPerson(node->left, id);
    }
}

/**
 * @brief           Create new node from person
 * @param person    Person pointer
 * @return          PersonNode
 */
PersonNode *createNewNode(Person *person)
{
    //Allocate memory for new Node
    PersonNode *result = (PersonNode *) malloc(sizeof(PersonNode));
    if (result != NULL)
    {
        result->person = person;
        result->right = NULL;
        result->left = NULL;
    }
    return result;
}

/**
 * @brief           Insert person to PersonNode BST
 * @param node      PersonNode node
 * @param person    Person pointer
 * @return          PersonNode root
 */
PersonNode *insert(PersonNode *node, Person *person)
{
    //Node is NULL
    if (node == NULL)
    {
        return createNewNode(person);
    }
    //Person Id smaller than current Node person id, try the left son
    if (person->id < node->person->id)
    {
        node->left = insert(node->left, person);
    }
        //Person Id bigger than current Node person id, try the left son
    else if (person->id > node->person->id)
    {
        node->right = insert(node->right, person);
    }
    return node;
}

/**
 * @brief           Main functions
 * @param argc      Arguments count
 * @param argv      Arguments
 * @return          EXIT_SUCCESS if succeeded, EXIT_FAILURE otherwise
 */
int main(int argc, char *argv[])
{
    int retValue = EXIT_SUCCESS;
    int treeFree = FALSE;
    if (argc != 3)
    {
        return EXIT_FAILURE;
    }
    FILE *peopleFile = fopen(argv[1], OPEN_FILE_PERMISSION);
    FILE *meetingFile = fopen(argv[2], OPEN_FILE_PERMISSION);

    //One of the files did not opened
    if (peopleFile == NULL || meetingFile == NULL)
    {
        fprintf(stderr, ERROR_IN_INPUT_FILE);
        return EXIT_FAILURE;
    }

    int readPeople = readPeopleFile(peopleFile);
    //Read people file got an error
    if (readPeople != SUCCESS)
    {
        retValue = EXIT_FAILURE;
        if (readPeople == ERROR)
        {
            fprintf(stderr, BAD_FORMAT_ERROR, argv[1], argv[2]);
        }
        else
        {
            fprintf(stderr, STANDARD_LIB_ERR_MSG);
        }
    }

    int readMeetings = readMeetingsFile(meetingFile);
    //Read meeting file got an error
    if (readMeetings != SUCCESS)
    {
        retValue = EXIT_FAILURE;
        if (readMeetings == ERROR)
        {
            fprintf(stderr, BAD_FORMAT_ERROR, argv[1], argv[2]);
        }
        else
        {
            fprintf(stderr, STANDARD_LIB_ERR_MSG);
        }
    }

    //If both file read well
    if (readPeople == SUCCESS && readMeetings == SUCCESS)
    {
        freeTree(peopleTree);
        treeFree = TRUE;
        sortPeoples();
        if (writeOutputFile(OUTPUT_FILE) != SUCCESS)
        {
            fprintf(stderr, ERROR_IN_OUTPUT_FILE);
            retValue = EXIT_FAILURE;
        }
    }
    freePeoples();
    if (treeFree == FALSE)
    {
        freeTree(peopleTree);
    }

    return retValue;

}