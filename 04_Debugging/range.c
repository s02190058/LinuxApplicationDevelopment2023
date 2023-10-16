#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int start = 0;
    int stop = 0;
    int step = 1;
    switch (argc)
    {
        case 1:
            printf("Usage: %s [START] STOP [STEP]\n", argv[0]);
            break;
        case 2:
            stop = atoi(argv[1]);
            break;
        case 4:
            step = atoi(argv[3]);
        case 3:
            start = atoi(argv[1]);
            stop = atoi(argv[2]);
            break;
        default:
            printf("%s: invalid number of arguments\n", argv[0]);
    }

    for (int i = start; i < stop; i += step) {
        printf("%d\n", i);
    }

    return 0;
}
