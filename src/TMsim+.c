#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pair_t { int _int; char _char; } pair;

typedef struct quadruplet_t { pair _read0; pair _read1; pair _read2; pair _read3; } state;

long long int init_memory = 1024; int max_states = 1024; 
unsigned long long int instruction_limit = (long long)4294967295; // Hey! 2 ** 32 - 1 instructions not enough for you? Change via command line!
char* memory; state* machine; char* space;

/// @brief This should help with memory deficiency.
/// @param mem The memory sent to be extended.
/// @return The memory sent, appended by an equal number of 0's.
char* double_memory(char* mem)
{
    printf("Trying to increase memory from %lli to %lli...\n", init_memory, 2 * init_memory);
    mem = realloc(mem, 2 * init_memory); if (mem == NULL) { printf("Doubling memory failed!\n"); return NULL; }

    for (int j = init_memory; j < 2 * init_memory; j++)
    {
        mem[j] = '\0';
    }

    init_memory *= 2;

    return mem; 
}

int main(int argc, char** argv)
{
    if (argc > 3) { printf("That's way too many arguments! We live in harmony here.\n"); return 0; }
    else if (argc == 3)
    {
        unsigned long long int commandline_instruction = (unsigned long long int)atoll(argv[1]);
        int commandline_maxstates = atoi(argv[2]);
        printf("The machine will run for %llu instructions maximum, and allow for a maximum of %i states.\n", commandline_instruction, commandline_maxstates); 
        instruction_limit = commandline_instruction;
        max_states = commandline_maxstates;
    }
    else if (argc == 2) 
    { 
        unsigned long long int commandline_instruction = (unsigned long long int)atoll(argv[1]);
        printf("The machine will run for %llu instructions maximum.\n", commandline_instruction); 
        instruction_limit = commandline_instruction;
    }
    else if (argc == 1)
    {
        printf("The machine will run for 4294967295 iterations maximum.\n");
    }

    state* machine = malloc(1024 * sizeof(state)); if (machine == NULL) { printf("Memory allocation 2 failed!\n"); return -1;}
    char* memory = malloc(1024 * sizeof(char)); if (memory == NULL) { printf("Memory allocation 1 failed!\n"); return -1;}

    
    for (int i = 0; i < 1024; i++) { memory[i] = (char)0; } // Initialize the array to empty chars

    // Reads from in-file to the machine.
    FILE* infile = fopen("Machine.txt","r");
    if (infile == NULL) { printf("An error occurred while opening machine!\n"); return -1; }

    int state0, state1, state2, state3; int counter = 0;
    char write0, write1, write2, write3;
    
    // Assigning the states into the array. %*[^\n]%*c ignores the trailing comments.
    while (fscanf(infile, "%i,%c,%i,%c,%i,%c,%i,%c%*[^\n]%*c", &state0, &write0, &state1, &write1, &state2, &write2, &state3, &write3) == 8)
    {
        state temporary = { ._read0 = { ._int = state0, ._char = write0 }, ._read1 = { ._int = state1, ._char = write1 }, ._read2 = { ._int = state2, ._char = write2 }, ._read3 = { ._int = state3, ._char = write3 } } ;
        if (counter >= max_states)
        {
            printf("Sorry, your machine has too many states... %i maximum, please!\n", max_states);
            return -2;
        }
        machine[counter] = temporary;
        counter += 1;
    }
    fclose(infile);

    if (counter == 0) { printf("The machine was null. Please give an actual machine!\n"); return -3; }

    
    // Next, write the intape into memory.
    FILE* intape = fopen("Intape.txt","r");
    if (intape == NULL) { printf("An error occurred while opening the input tape!\n"); return -1;}
    char symbol; int mempos = 0;

    // Writes the contents of the file into the memory array.
    for (;;)
    {
        symbol = fgetc(intape);
        if (mempos >= init_memory)
        {
            memory = double_memory(memory);
        }

        if (symbol == '\n' || symbol == EOF)
        {
            break;
        }
        memory[mempos] = symbol;
        mempos += 1;
    }

    fclose(intape);

    // Simulates the Turing Machine.

    long long int headlocation = 0; int currentindex = 0; char symbolread = '\0'; int haltstate = counter - 1;
    state currentstate; unsigned long long int iteration = 0; int returncode = 0;

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
                else if (currentstate._read0._char == 'R') { headlocation += 1; if (headlocation >= init_memory) { memory = double_memory(memory); } } // I guess we didn't give you enough memory...
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
                else if (currentstate._read1._char == 'R') { headlocation += 1; if (headlocation >= init_memory) { memory = double_memory(memory); } } // I guess we didn't give you enough memory...
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
                else if (currentstate._read2._char == 'R') { headlocation += 1; if (headlocation >= init_memory) { memory = double_memory(memory); } } // I guess we didn't give you enough memory...
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
                else if (currentstate._read3._char == 'R') { headlocation += 1; if (headlocation >= init_memory) { memory = double_memory(memory); } } // I guess we didn't give you enough memory...
                else { memory[headlocation] = currentstate._read3._char == '3' ? '\0' : currentstate._read3._char; } // Write into the location.
            }
            currentindex = currentstate._read3._int;
        }
        else { returncode = -4; break;} // An illegal character was read.
        if (memory == NULL) { returncode = -3; break; } // Out of memory. For real.
        iteration += 1; 
        if (iteration == instruction_limit) { returncode = -5; break; }
        // printf("%s\n", memory); // For debugging, we can ask it to print the memory.
        /* Uncomment this region to make it print the transition sequence up to a certain point.
        for (int j = 0; j < 32; j++)
        {
            if (memory[j] == (char)0) { putchar(' '); } else { putchar(memory[j]); };
        }
        printf(" %i %i\n", currentindex, headlocation);
        */
    }
    // The error messages can be...
    if (returncode != 0)
    {
        if (returncode == -1) { printf("Halted in state %i.\n", currentindex); }
        else if (returncode == -2) { printf("Machine fell off the left edge.\n"); }
        else if (returncode == -3) { printf("Out of memory.\n"); }
        else if (returncode == -4) { printf("Illegal character read. Terminating...\n"); }
        else if (returncode == -5) { printf("Machine timed out.\n"); }
        
        printf("Error occurred on iteration %llu, memory location %lli, while the machine was in state %i, reading character %c.\n", iteration, headlocation, currentindex, symbolread);
        /* Diagnostic dump written to tape. Uncomment to use.
        for (int j = 0; j < init_memory; j++)
        {
            if (memory[j] == (char)0) { putchar(' '); } else { putchar(memory[j]); };
        }
        */
    }

    if (currentindex > haltstate) { printf("The machine was partial. Don't skip any states!\n"); return -4; }
    // Otherwise, a computation has been completed.

    if (headlocation == 0) { printf("The head was parked.\n"); if (returncode == 0) { printf("The computation completed after %llu iterations.\n", iteration); }}

    // Opens the outfile for writing.

    FILE* outfile = fopen("Outtape.txt","w"); if (outfile == NULL) { printf("An error occurred while writing to output!\n"); return -1;}


    fprintf(outfile, "%s\n", memory);

    fclose(outfile);
    free(memory);
    free(machine);

    printf("Total memory used: %lli\n", init_memory);
    return 0;
}


