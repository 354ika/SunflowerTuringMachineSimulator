#include <stdio.h>
#include <stdlib.h>

FILE* machine; FILE* modify;

int main(int argc, char** argv)
{
    if (argc != 2) { printf("Please use with 2 arguments. The second one is for the shift parameter.\n"); return -1; }

    int shift = atoi(argv[1]);

    machine = fopen("Machine.txt","r"); if (machine == NULL) { printf("Opening machine file failed!\n"); return -1; }
    modify = fopen("Modify.txt","w"); if (modify == NULL) { printf("Opening modify destination failed!\n"); return -1; }

    int state0, state1, state2, state3; int lines = 0;
    char write0, write1, write2, write3; 
    char* comment = malloc(128); int a;

    while ((a = fscanf(machine, "%i,%c,%i,%c,%i,%c,%i,%c%[^\n]%*c", &state0, &write0, &state1, &write1, &state2, &write2, &state3, &write3, comment)) && a >= 8) 
    {
        if (state0 != -1) { state0 += shift; } if (state1 != -1) { state1 += shift; } if (state2 != -1) { state2 += shift; } if (state3 != -1) { state3 += shift; } 
        if (a == 9) { fprintf(modify, "%i,%c,%i,%c,%i,%c,%i,%c%s\n", state0, write0, state1, write1, state2, write2, state3, write3, comment); }
        else { fprintf(modify, "%i,%c,%i,%c,%i,%c,%i,%c\n", state0, write0, state1, write1, state2, write2, state3, write3); }
        lines += 1;
    }

    printf("%i lines shifted by %i.\n", lines, shift);
    
    fclose(modify);
    fclose(machine);
    return 0;
}