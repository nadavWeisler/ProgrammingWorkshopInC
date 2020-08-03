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
#define TRUE 0 == 0
#define FALSE 0 == 1
#define CONVERSION_BASE 10

/**
 * @brief Struct that define a peron
 */
typedef struct person
{
    char *name;
    long id;
    float age;
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

PersonNode *peopleTree;
Person **peoples;
int peopleCount;

/**
 * @brief           Free root
 * @param root      Person node
 */
void freeTree(PersonNode *root);

/**
 * @brief           Find person ib PersonNode BST
 * @param node      BST node
 * @param id        Person id (long)
 * @return          Person pointer
 */
Person *findPerson(PersonNode *node, long id);

/**
 * @brief           Create new node from person
 * @param person    Person pointer
 * @return          PersonNode
 */
PersonNode *createNewNode(Person *person);

/**
 * @brief Free memory
 */
void freeMain();

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
float crna(float distance, float time);

/**
 * @brief           Sort people collection by quick sort algorithm
 * @param low       Low index
 * @param high      High index
 */
void quickSort(int low, int high);

/**
 * @brief           Calculate partition for quick sort algorithm
 * @param low       Low index
 * @param high      High index
 * @return
 */
int partition(int low, int high);

/**
 * @brief           Update person value by file line
 * @param person    Person pointer
 * @param line      Line (string)
 */
void updatePersonFromLine(Person *person, char *line);

/**
 * @brief           Update person infection from line from meeting file
 * @param line      Line from meeting file (string)
 */
void updatePersonInfectedFromLine(char *line);

/**
 * @brief           Read people in file
 * @param file      People in file pointer
 * @return          TRUE is succeeded, FALSE otherwise
 */
int readPeopleFile(FILE *file);

/**
 * @brief Sort people (quicksort algorithm)
 */
void sortPeoples();

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
void writeOutputFile(char *outputFilePath);

/**
 * @brief               Infection probability formula
 * @param distance      Distance (float)
 * @param time          Time (float)
 * @return              Infection probability (float)
 */
float crna(float distance, float time)
{
    return (time * MIN_DISTANCE) / (distance * MAX_TIME);
}

/**
 * @brief           Update person value by file line
 * @param person    Person pointer
 * @param line      Line (string)
 */
void updatePersonFromLine(Person *person, char *line)
{
    char *splitLine = strtok(line, " ");
    int count = 0;
    char **literal = NULL;
    while (splitLine != NULL)
    {
        if (count == 0)
        {
            person->name = malloc(strlen(splitLine) + 1);
            strcpy(person->name, splitLine);
        }
        else if (count == 1)
        {
            person->id = strtol(splitLine, literal, CONVERSION_BASE);
        }
        else if (count == 2)
        {
            person->age = strtof(splitLine, literal);
        }
        count++;
        splitLine = strtok(NULL, " ");
    }
    person->chance = 0;
}

/**
 * @brief           Update person infection from line from meeting file
 * @param line      Line from meeting file (string)
 */
void updatePersonInfectedFromLine(char *line)
{
    char *splitLine = strtok(line, " ");
    int count = 0;
    char **literal = NULL;
    Person *infector;
    Person *infected;
    float distance;
    float time;
    long currentId;
    while (splitLine != NULL)
    {
        if (count == 0)
        {
            currentId = strtol(splitLine, literal, CONVERSION_BASE);
            infector = findPerson(peopleTree, currentId);
        }
        else if (count == 1)
        {
            currentId = strtol(splitLine, literal, CONVERSION_BASE);
            infected = findPerson(peopleTree, currentId);
        }
        else if (count == 2)
        {
            distance = strtof(splitLine, literal);
        }
        else if (count == 3)
        {
            time = strtof(splitLine, literal);
        }
        count++;
        splitLine = strtok(NULL, " ");
    }

    infected->chance = crna(distance, time);
    if (infector->chance != 0)
    {
        infected->chance *= infector->chance;
    }
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
    Person *newPerson;
    while (fgets(line, LINE_LENGTH, file))
    {
        if (line[0] == NEW_LINE_CHAR)
        {
            continue;
        }
        removeNewLineChar(line);
        newPerson = (Person *) malloc(sizeof(Person));
        updatePersonFromLine(newPerson, line);
        if (newPerson != NULL)
        {
            peopleTree = insert(peopleTree, newPerson);
            peopleCount++;
            peoples = (Person **) realloc(peoples, peopleCount * sizeof(Person *));
            peoples[peopleCount - 1] = newPerson;
        }
        else
        {
            return FALSE;
        }
    }
    return TRUE;
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
    long currentId;
    Person *currentPerson;
    char **literal = NULL;

    while (fgets(line, LINE_LENGTH, file))
    {
        removeNewLineChar(line);
        if (firstLine)
        {
            currentId = strtol(line, literal, CONVERSION_BASE);
            currentPerson = findPerson(peopleTree, currentId);
            currentPerson->chance = 1;
            firstLine = FALSE;
        }
        else
        {
            if (line[0] == NEW_LINE_CHAR)
            {
                continue;
            }
            updatePersonInfectedFromLine(line);
        }
    }
    return TRUE;
}

/**
 * @brief                   Write to output file
 * @param outputFilePath    Output file path
 */
void writeOutputFile(char *outputFilePath)
{
    FILE *outputFile = fopen(outputFilePath, WRITE_FILE_PERMISSION);
    Person *currentPerson;
    char *currentMsg;
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
            free(peoples[i]->name);
            free(peoples[i]);
        }
        free(peoples);
    }
    peoples = NULL;
}

/**
 * @brief           Calculate partition for quick sort algorithm
 * @param low       Low index
 * @param high      High index
 * @return
 */
int partition(int low, int high)
{
    Person *pivot = peoples[high];
    int wall = low;
    for (int i = low; i < high; i++)
    {
        if (peoples[i]->chance > pivot->chance)
        {
            Person *temp = peoples[wall];
            peoples[wall] = peoples[i];
            peoples[i] = temp;
            wall++;
        }
    }
    peoples[high] = peoples[wall];
    peoples[wall] = pivot;
    return wall;
}

/**
 * @brief           Sort people collection by quick sort algorithm
 * @param low       Low index
 * @param high      High index
 */
void quickSort(int low, int high)
{
    if (low < high)
    {
        int pivotIndex = partition(low, high);
        quickSort(low, pivotIndex - 1);
        quickSort(pivotIndex + 1, high);
    }
}

/**
 * @brief Sort people (quicksort algorithm)
 */
void sortPeoples()
{
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
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

/**
 * @brief           Find person ib PersonNode BST
 * @param node      BST node
 * @param id        Person id (long)
 * @return          Person pointer
 */
Person *findPerson(PersonNode *node, long id)
{
    if (node == NULL || node->person->id == id)
    {
        return node->person;
    }

    if (node->person->id < id)
    {
        return findPerson(node->right, id);
    }
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
    PersonNode *result = (PersonNode *) malloc(sizeof(PersonNode));
    result->person = person;
    result->right = NULL;
    result->left = NULL;
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
    if (node == NULL)
    {
        return createNewNode(person);
    }
    if (person->id < node->person->id)
    {
        node->left = insert(node->left, person);
    }
    else if (person->id > node->person->id)
    {
        node->right = insert(node->right, person);
    }
    return node;
}

/**
 * @brief Free memory
 */
void freeMain()
{
    freePeoples();
    freeTree(peopleTree);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        return EXIT_FAILURE;
    }
    FILE *peopleFile = fopen(argv[1], OPEN_FILE_PERMISSION);
    FILE *meetingFile = fopen(argv[2], OPEN_FILE_PERMISSION);
    if (peopleFile == NULL || meetingFile == NULL)
    {
        return EXIT_FAILURE;
    }
    if (readPeopleFile(peopleFile) && readMeetingsFile(meetingFile))
    {
        sortPeoples();
        writeOutputFile(OUTPUT_FILE);
        freeMain();
        return EXIT_SUCCESS;
    }
    else
    {
        freeMain();
        return EXIT_FAILURE;
    }
}