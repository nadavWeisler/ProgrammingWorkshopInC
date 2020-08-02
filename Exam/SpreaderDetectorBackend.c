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

#include "SpreaderDetectorParams.h"

typedef struct infection
{
    float distance;
    float time;
    char *infectedId;
} Infection;

typedef struct person
{
    char *name;
    char *id;
    float age;
    struct infection *infections;
    int infectionCount;
    float chance;
} Person;

Person **peoples;
int peopleCount;
float *chanceMatrix;

Person *getPersonFromId(const char *id);

void RemoveNewLineChar(char *line);

int getMatIndex(int i, int j, int n);

void freePeoples();

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
            person->id = strdup(splitLine);
        }
        else if (count == 2)
        {
            person->age = strtof(splitLine, literal);
        }
        count++;
        splitLine = strtok(NULL, " ");
    }
    person->infections = NULL;
    person->infectionCount = 0;
    person->chance = 0;
}

void getInfectionFromLine(Person *currentPerson, char *line)
{
    char *splitLine = strtok(line, " ");
    int count = 0;
    char **literal = NULL;
    Person *infector = currentPerson;
    Person *infected;
    Infection infection;
    while (splitLine != NULL)
    {
        if (count == 0)
        {
            if (splitLine != infector->id)
            {
                infector = getPersonFromId(splitLine);
            }
        }
        else if (count == 1)
        {
            infected = getPersonFromId(splitLine);
            infection.infectedId = infected->id;
        }
        else if (count == 2)
        {
            infection.distance = strtof(splitLine, literal);
        }
        else if (count == 3)
        {
            infection.time = strtof(splitLine, literal);
        }
        count++;
        splitLine = strtok(NULL, " ");
    }

    infector->infectionCount++;
    infector->infections = (Infection *) realloc(infector->infections,
                                                 sizeof(Infection) *
                                                 infector->infectionCount);
    infector->infections[infector->infectionCount - 1] = infection;
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
    while (splitLine != NULL)
    {
        if (count == 0)
        {
            infector = getPersonFromId(splitLine);
        }
        else if (count == 1)
        {
            infected = getPersonFromId(splitLine);
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

    infected->chance = infector->chance * crna(distance, time);
}

Person *getPersonFromId(const char *id)
{
    for (int i = 0; i < peopleCount; i++)
    {
        if (strcmp(peoples[i]->id, id) == 0)
        {
            return peoples[i];
        }
    }
    return NULL;
}

void printPerson(Person *p)
{
    printf("%s %s %f %d\n", p->name, p->id, p->age, p->infectionCount);
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

void RemoveBadChar(char *line)
{
    unsigned long strLen = strlen(line);
    if (line[strLen - 1] == BACKSLASH_R)
    {
        line[strLen - 1] = '\0';
    }
}

float getChance(Person *infector, Person *infected)
{
    if (infector->id != infected->id)
    {
        for (int i = 0; i < infector->infectionCount; i++)
        {
            if (infector->infections[i].infectedId == infected->id)
            {
                return crna(infector->infections[i].distance,
                            infector->infections[i].time);
            }
        }
    }
    return 0;
}

void UpdateChanceMatrix()
{
    chanceMatrix = (float *) malloc(sizeof(float) * peopleCount * peopleCount);
    for (int i = 0; i < peopleCount; i++)
    {
        for (int j = 0; j < peopleCount; j++)
        {
            chanceMatrix[getMatIndex(i,j, peopleCount)] = getChance(peoples[i], peoples[j]);
        }
    }
}

int readMeetingsFile(FILE *file)
{
    char line[LINE_LENGTH];
    int firstLine = TRUE;
    char *currentId;
    Person *currentPerson;
    while (fgets(line, LINE_LENGTH, file))
    {
        RemoveNewLineChar(line);
        if (firstLine)
        {
            currentId = strdup(line);
            RemoveNewLineChar(currentId);
            currentPerson = getPersonFromId(currentId);
            firstLine = FALSE;
        }
        else
        {
            if (line[0] == NEW_LINE_CHAR)
            {
                continue;
            }
            getInfectionFromLine(currentPerson, line);
        }
    }
    return TRUE;
}

void writeOutputFile(char *outputFilePath)
{
    FILE *outputFile = fopen(outputFilePath, WRITE_FILE_PERMISSION);
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

int getMatIndex(int i, int j, int n)
{
    return i * n + j;
}

void printMatrix()
{
    for (int i = 0; i < peopleCount; i++)
    {
        for (int j = 0; j < peopleCount; j++)
        {
            printf(" %f ", chanceMatrix[getMatIndex(i,j, peopleCount)]);
        }
        printf("\n");
    }
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
        printAllPeople();
        UpdateChanceMatrix();
        printMatrix();
        freeMain();
        return EXIT_SUCCESS;
    }
    else
    {
        freeMain();
        return EXIT_FAILURE;
    }
}