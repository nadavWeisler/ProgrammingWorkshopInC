#include <stdio.h>
#include <stdlib.h>

/** Lab1 Main function **/
int main(int argc, char *argv[])
{
    if (argc == 2)
    {
        int arg = atoi(argv[1]);
        if (arg == 1)
        {
            printf("Hello, World!");
        }
        else if (arg == 2)
        {
            printf("Welcome to C!");
        }
    }
    return 0;
}

