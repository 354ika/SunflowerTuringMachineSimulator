#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pair_t { int _int; char _char; } pair;

typedef struct quadruplet_t { pair _read0; pair _read1; pair _read2; pair _read3; } state;

int init_memory = 1024; int max_states = 1024; int instruction_limit = 2147483647;
char* memory; state* machine;

int main(void)
{
    char* memory = malloc(1024 * sizeof(char)); if (memory == NULL) { printf("Memory allocation 1 failed!\n"); return -1;}
    state* machine = malloc(1024 * sizeof(state)); if (machine == NULL) { printf("Memory allocation 2 failed!\n"); return -1;}

    
    for (int i = 0; i < 1024; i++) { memory[i] = (char)0; } // Initialize the array to empty chars

    // Reads from in-file to the machine.
    FILE* infile = fopen("Machine.txt","r");
    if (infile == NULL) { printf("An error occurred while opening machine!\n"); return -1; }

    int state0, state1, state2, state3; int counter = 0;
    char write0, write1, write2, write3;
    
    // Assigning the states into the array
    while (fscanf(infile, "%i,%c,%i,%c,%i,%c,%i,%c", &state0, &write0, &state1, &write1, &state2, &write2, &state3, &write3) == 8)
    {
        state temporary = { ._read0 = { ._int = state0, ._char = write0 }, ._read1 = { ._int = state1, ._char = write1 }, ._read2 = { ._int = state2, ._char = write2 }, ._read3 = { ._int = state3, ._char = write3 } } ;
        if (counter >= 1024)
        {
            printf("Sorry, your machine has too many states... 1024 maximum, please!\n");
            return -2;
        }
        machine[counter] = temporary;
        counter += 1;
    }
    fclose(infile);

    if (counter == 0) { printf("The machine was null. Please give an actual machine!"); return -3; }

    
    // Next, write the intape into memory.
    FILE* intape = fopen("Intape.txt","r");
    if (intape == NULL) { printf("An error occurred while opening the input tape!\n"); return -1;}
    char symbol; int mempos = 0;

    // Writes the contents of the file into the memory array.
    for (;;)
    {
        symbol = fgetc(intape);
        if (symbol == '\n' || symbol == EOF)
        {
            break;
        }
        memory[mempos] = symbol;
        mempos += 1;
    }

    fclose(intape);

    // Simulates the Turing Machine.

    int headlocation = 0; int currentindex = 0; char symbolread = '\0'; int haltstate = counter - 1;
    state currentstate; int iteration = 0; int returncode = 0;

    while (currentindex < haltstate)
    {
        symbolread = memory[headlocation]; // Which symbol did I read?
        currentstate = machine[currentindex]; // Which state is the machine in?
        if (symbolread == '0') // A zero character is read.
        {
            if (currentstate._read0._int == -1) { returncode = -1; break; } // Halted while not in the halt state.
            else
            {
                if (currentstate._read0._char == 'L') { headlocation -= 1; if (headlocation < 0) { returncode = -2; break; } } // Machine in inadmissible state.
                else if (currentstate._read0._char == 'R') { headlocation += 1; if (headlocation >= 1024) { returncode = -3; break; } } // I guess we didn't give you enough memory...
                else { memory[headlocation] = currentstate._read0._char == '3' ? '\0' : currentstate._read0._char; } // Write into the location.
            }
            currentindex = currentstate._read0._int;
        }
        else if (symbolread == '1') // A one character is read.
        {
            if (currentstate._read1._int == -1) { returncode = -1; break; } // Halted while not in the halt state.
            else
            {
                if (currentstate._read1._char == 'L') { headlocation -= 1; if (headlocation < 0) { returncode = -2; break; } } // Machine in inadmissible state.
                else if (currentstate._read1._char == 'R') { headlocation += 1; if (headlocation >= 1024) { returncode = -3; break; } } // I guess we didn't give you enough memory...
                else { memory[headlocation] = currentstate._read1._char == '3' ? '\0' : currentstate._read1._char; } // Write into the location.
            }
            currentindex = currentstate._read1._int;
        }
        else if (symbolread == '2') // The string delimiter is read.
        {
            if (currentstate._read2._int == -1) { returncode = -1; break; } // Halted while not in the halt state.
            else
            {
                if (currentstate._read2._char == 'L') { headlocation -= 1; if (headlocation < 0) { returncode = -2; break; } } // Machine in inadmissible state.
                else if (currentstate._read2._char == 'R') { headlocation += 1; if (headlocation >= 1024) { returncode = -3; break; } } // I guess we didn't give you enough memory...
                else { memory[headlocation] = currentstate._read2._char == '3' ? '\0' : currentstate._read2._char; } // Write into the location.
            }
            currentindex = currentstate._read2._int;
        }
        else if (symbolread == '\0')// The null character is read.
        {
            if (currentstate._read3._int == -1) { returncode = -1; break; } // Halted while not in the halt state.
            else
            {
                if (currentstate._read3._char == 'L') { headlocation -= 1; if (headlocation < 0) { returncode = -2; break; } } // Machine in inadmissible state.
                else if (currentstate._read3._char == 'R') { headlocation += 1; if (headlocation >= 1024) { returncode = -3; break; } } // I guess we didn't give you enough memory...
                else { memory[headlocation] = currentstate._read3._char == '3' ? '\0' : currentstate._read3._char; } // Write into the location.
            }
            currentindex = currentstate._read3._int;
        }
        else { returncode = -4; break;} // What? Don't want to play by the rules?
        iteration += 1; 
        if (iteration == instruction_limit) { returncode = -5; break; }
        // printf("%s\n", memory); // For debugging, we can ask it to print the memory.
    }
    // The error messages can be...
    if (returncode != 0)
    {
        if (returncode == -1) { printf("Halted in state %i.\n", currentindex); }
        else if (returncode == -2) { printf("Machine fell off the left edge.\n"); }
        else if (returncode == -3) { printf("Out of memory.\n"); }
        else if (returncode == -4) { printf("Illegal character read. Terminating...\n"); }
        else if (returncode == -5) { printf("Machine timed out.\n"); }
        printf("Error occurred on iteration %i, memory location %i, while the machine was in state %i, reading character %c.\n", iteration, headlocation, currentindex, symbolread);
    }

    if (currentindex > haltstate) { printf("The machine was partial. Don't skip any states!\n"); return -4; }
    // Otherwise, a computation has been completed.

    if (headlocation == 0) { printf("The head was parked.\n"); if (returncode == 0) { printf("The computation completed after %i iterations.\n", iteration); }}

    // Opens the outfile for writing.

    FILE* outfile = fopen("Outtape.txt","w"); if (outfile == NULL) { printf("An error occurred while writing to output!\n"); return -1;}


    fprintf(outfile, "%s\n", memory);

    fclose(outfile);
    free(memory);
    free(machine);
    return 0;
}


