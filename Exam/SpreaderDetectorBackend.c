#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OUTPUT_FILE "SpreaderDetectorAnalysis.out"
#define OPEN_FILE_PERMISSION "r"
#define WRITE_FILE_PERMISSION "w"
#define NEW_LINE_CHAR '\n'
#define BACKSLASH_R '\r'
#define LINE_LENGTH 1024
#define TRUE 0==0
#define FALSE 0==1
#define CONVERSION_BASE 10

#include "SpreaderDetectorParams.h"

typedef struct person
{
    char *name;
    long id;
    float age;
    float chance;
} Person;

Person **peoples;
int peopleCount;

Person *getPersonFromId(long id);

void RemoveNewLineChar(char *line);

void freePeoples();

float crna(float distance, float time);

void printPerson(Person *p);

void updatePersonFromLine(Person *person, char *line);

void updatePersonInfectedFromLine(char *line);

int readPeopleFile(FILE *file);

int readMeetingsFile(FILE *file);

void freeMain();

void printAllPeople();

void merge(int startIndex, int middleIndex, int endIndex);

void mergeSort(int startIndex, int endIndex);

void writeOutputFile(char *outputFilePath);

float crna(float distance, float time)
{
    return (time * MIN_DISTANCE) / (distance * MAX_TIME);
}

void updatePersonFromLine(Person *person, char *line)
{
    char *splitLine = strtok(line, " ");
    int count = 0;
    char **literal = NULL;
    while (splitLine != NULL)
    {
        if (count == 0)
        {
            person->name = strdup(splitLine);
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
            infector = getPersonFromId(currentId);
        }
        else if (count == 1)
        {
            currentId = strtol(splitLine, literal, CONVERSION_BASE);
            infected = getPersonFromId(currentId);
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

Person *getPersonFromId(long id)
{
    for (int i = 0; i < peopleCount; i++)
    {
        if (peoples[i]->id == id)
        {
            return peoples[i];
        }
    }
    return NULL;
}

void printPerson(Person *p)
{
    printf("%s %lu %f %f\n", p->name, p->id, p->age, p->chance);
}

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
        RemoveNewLineChar(line);
        newPerson = (Person *) malloc(sizeof(Person));
        updatePersonFromLine(newPerson, line);
        if (newPerson != NULL)
        {
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

void RemoveNewLineChar(char *line)
{
    unsigned long strLen = strlen(line);
    if (line[strLen - 1] == NEW_LINE_CHAR || line[strLen - 1] == BACKSLASH_R)
    {
        line[strLen - 1] = '\0';
    }
}

int readMeetingsFile(FILE *file)
{
    char line[LINE_LENGTH];
    int firstLine = TRUE;
    long currentId;
    Person *currentPerson;
    char **literal = NULL;

    while (fgets(line, LINE_LENGTH, file))
    {
        RemoveNewLineChar(line);
        if (firstLine)
        {
            currentId = strtol(line, literal, CONVERSION_BASE);
            currentPerson = getPersonFromId(currentId);
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

void freeMain()
{
    printf("Free!\n");
    freePeoples();
}

void freePeoples()
{
    if (peopleCount > 0)
    {
        for (int i = 0; i < peopleCount; i++)
        {
            free(peoples[i]);
        }
        free(peoples);
    }
    peoples = NULL;
}

void printAllPeople()
{
    for (int i = 0; i < peopleCount; i++)
    {
        printf("---\n");
        printPerson(peoples[i]);
        printf("---\n");
    }
}

int partition(int low, int high)
{
    Person* pivot = peoples[high];
    int wall = low;
    for (int i = low; i < high; i++)
    {
        if (peoples[i]->chance > pivot->chance)
        {
            Person* temp = peoples[wall];
            peoples[wall] = peoples[i];
            peoples[i] = temp;
            wall++;
        }
    }
    peoples[high] = peoples[wall];
    peoples[wall] = pivot;
    return wall;
}

void quickSort(int low, int high)
{
    if (low < high)
    {
        int pivotIndex = partition(low, high);
        quickSort(low, pivotIndex - 1);
        quickSort(pivotIndex + 1, high);
    }
}

void sortPeoples()
{
    quickSort(0, peopleCount - 1);
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
        printAllPeople();
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