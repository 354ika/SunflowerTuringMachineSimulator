#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    int size = 0; 
    if (argc > 2) { printf("Too many arguments!\n"); return -1; }
    else if (argc == 1) { size = 128; }
    else { size = atoi(argv[1]); }

    FILE* intape = fopen("Intape.txt","w");


    for (int i = 1; i < size; i++)
    {
        fputc('2', intape);
        if (i == 0) { fputc('0', intape); } 
        else 
        {
            int j = i;
            
            while (j != 0)
            {
                int r = j % 2;
                fputc((48 + r), intape);
                j = j >> 1;
            }
        }
    }

    fclose(intape);
    fputs("Writing to input complete!\n", stdout);
    return 0;
}