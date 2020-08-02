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

struct infection
{
    float distance;
    float time;
    char *infectedId;
};

typedef struct person
{
    char *name;
    char *id;
    float age;
    struct infection *infections;
    int infectionCountForPerson;
} Person;

typedef struct meeting
{
    char *infectorId;
    char *infectedId;
    float distance;
    float time;
} Meeting;

typedef struct coronaTree
{
    Person *infector;
    Person *infected;
    float distance;
    float time;
} Infection;

Person **peoples;
int peopleCount;
Meeting ***meeting;
int *meetingCount;
int meetingsGroupsCount;
Infection **infections;
int infectionsCount = 0;

Person *getPersonFromId(const char *id);

void RemoveNewLineChar(char *line);

void RemoveBadChar(char *line);

void freePeoples();

void freeMeetings();

double crna(double distance, double time)
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
        switch (count)
        {
            case 0:
                person->name = strdup(splitLine);
                break;
            case 1:
                person->id = strdup(splitLine);
                break;
            case 2:
                person->age = strtof(splitLine, literal);
                break;
        }
        count++;
        splitLine = strtok(NULL, " ");
    }
}

Meeting *getMeetingFromLine(char *line)
{
    Meeting *newMeeting = (Meeting *) malloc(sizeof(Meeting));
    char *splitLine = strtok(line, " ");
    int count = 0;
    char **literal = NULL;
    while (splitLine != NULL)
    {
        switch (count)
        {
            case 0:
                newMeeting->infectorId = strdup(splitLine);
                break;
            case 1:
                newMeeting->infectedId = strdup(splitLine);
                break;
            case 2:
                newMeeting->distance = strtof(splitLine, literal);
                break;
            case 3:
                newMeeting->time = strtof(splitLine, literal);
                break;
            default:
                return NULL;
        }
        count++;
        splitLine = strtok(NULL, " ");
    }
    return newMeeting;
}

Infection *getInfectionFromLine(Person *currentPerson, char *line)
{
    Infection *newInfection = (Infection *) malloc(sizeof(Infection));
    char *splitLine = strtok(line, " ");
    int count = 0;
    char **literal = NULL;
    Person *person = currentPerson;
    struct infection infection;
    while (splitLine != NULL)
    {
        switch (count)
        {
            case 0:
                if (splitLine == person->id)
                {
                    newInfection->infector = currentPerson;
                }
                else
                {
                    newInfection->infector = getPersonFromId(splitLine);
                }
                break;
            case 1:
                newInfection->infected = getPersonFromId(splitLine);
                infection.infectedId = newInfection->infected->id;
                break;
            case 2:
                newInfection->distance = strtof(splitLine, literal);
                infection.distance = strtof(splitLine, literal);
                break;
            case 3:
                newInfection->time = strtof(splitLine, literal);
                infection.time = strtof(splitLine, literal);
                break;
            default:
                return NULL;
        }
        count++;
        splitLine = strtok(NULL, " ");
    }
    newInfection->infector->infectionCountForPerson++;
    newInfection->infector->infections = (struct infection *) realloc(
            newInfection->infector->infections,
            newInfection->infector->infectionCountForPerson *
            sizeof(struct infection));
    newInfection->infector->infections[newInfection->infector->infectionCountForPerson - 1] =
            infection;
    return newInfection;
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
    printf("%s %s %f %d\n", p->name, p->id, p->age, p->infectionCountForPerson);
}

void printMeeting(Meeting *m)
{
    printf("%s %s %f %f\n", m->infectorId, m->infectedId, m->distance, m->time);
}

void printInfection(Infection *i)
{
    printf("-------\n");
    printPerson(i->infector);
    printPerson(i->infected);
    printf("%f %f\n", i->distance, i->time);
    printf("-------\n");
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

int readMeetingsFileOld(FILE *file)
{
    char line[LINE_LENGTH];
    meeting = (Meeting ***) malloc(0);
    int firstLine = TRUE;
    char *currentId;
    while (fgets(line, LINE_LENGTH, file))
    {
        RemoveNewLineChar(line);
        if (firstLine)
        {
            currentId = strdup(line);
            meetingsGroupsCount = 1;
            meetingCount = (int *) malloc(sizeof(int));
            meeting[0] = (Meeting **) malloc(0);
            meetingCount[0] = 0;
            firstLine = FALSE;
        }
        else
        {
            if (line[0] == NEW_LINE_CHAR)
            {
                continue;
            }
            Meeting *newMeeting = getMeetingFromLine(line);
            if (newMeeting != NULL)
            {
                if (newMeeting->infectorId == currentId)
                {
                    meetingCount[meetingsGroupsCount - 1]++;
                    meeting[meetingsGroupsCount - 1] =
                            (Meeting **) realloc(meeting[meetingsGroupsCount - 1],
                                                 meetingCount[meetingsGroupsCount - 1] *
                                                 sizeof(Meeting *));
                }
                else
                {
                    meetingsGroupsCount++;
                    meetingCount = (int *) realloc(meetingCount,
                                                   meetingsGroupsCount * sizeof(int));
                    meetingCount[meetingsGroupsCount - 1] = 1;
                    meeting[meetingsGroupsCount - 1] = (Meeting **) malloc(sizeof(Meeting *));

                }
                meeting[meetingsGroupsCount - 1][meetingCount[meetingsGroupsCount - 1]] =
                        newMeeting;
            }
            else
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}

int readMeetingsFile(FILE *file)
{
    char line[LINE_LENGTH];
    infections = (Infection **) malloc(0);
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
            Infection *newInfection = getInfectionFromLine(currentPerson, line);

            if (newInfection != NULL)
            {
                infectionsCount++;
                infections = (Infection **) realloc(infections,
                                                    sizeof(Infection *) * infectionsCount);
                infections[infectionsCount - 1] = newInfection;
            }
            else
            {
                return FALSE;
            }
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
    freeMeetings();
}

void freeMeetings()
{
    if (meetingCount > 0)
    {
        for (int i = 0; i < meetingsGroupsCount; i++)
        {
            for (int j = 0; j < meetingCount[i]; j++)
            {
                free(meeting[i][j]);
            }
            free(meeting[i]);
        }
        free(meeting);
    }
    meeting = NULL;
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

void printAllInfections()
{
    for (int i = 0; i < infectionsCount; i++)
    {
        printInfection(infections[i]);
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
        printAllInfections();
        freeMain();
        return EXIT_SUCCESS;
    }
    else
    {
        freeMain();
        return EXIT_FAILURE;
    }
}