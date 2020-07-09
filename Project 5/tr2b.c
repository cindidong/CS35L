//tr2b.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void translateFile(char* file, char* from, char* to, int size) {
    size_t k = 0;
    size_t l;
    while(k < size) {
        for (l = 0; from[l] != '\0'; l++) {
            if (file[k] == from[l])
            {
                file[k] = to[l];
                break;
            }
        }
        putchar(file[k]);
        if (ferror(stdout) != 0)
        {
            fprintf(stderr, "Stdout error\n");
            exit(1);
        }
        k++;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "There needs to be 3 operands\n");
        exit(1);
    }
    char* from = argv[1];
    char* to = argv[2];
    size_t fromSize = strlen(from);
    size_t toSize = strlen(to);
    if (fromSize != toSize) {
        fprintf(stderr, "Operands need to be the same length\n");
        exit(1);
    }
    //searching for duplicates
    int table[255];
    size_t i;
    for (i = 0; from[i] != '\0'; i++) {
        if (table[(int)from[i]] == 1) {
            fprintf(stderr, "There are duplicates\n");
            exit(1);
        }
        table[(int)from[i]] = 1;
    }
    char* file = (char*) malloc(sizeof(char));
    char c;
    int j = 0;
    const int buffSize = 10000;
    while ((c = getchar()) != EOF) {
        if (ferror(stdin) != 0) {
            fprintf(stderr, "Stdin error\n");
            exit(1);
        }
        if (j < buffSize) {
            char* temp = realloc(file, (j+1)*sizeof(char));
            if (temp == NULL) {
                free(file);
                fprintf(stderr, "Memory allocation error\n");
                exit(1);
            }
            file = temp;
            file[j] = c;
            j++;
        }
        //make space in memory in the case of large files
        else
        {
            char* temp = realloc(file, (j+1)*sizeof(char));
            if (temp == NULL)
            {
                free(file);
                fprintf(stderr, "Memory allocation error\n");
                exit(1);
            }
            file = temp;
            file[j] = c;
            j++;
            translateFile(file, from, to, buffSize + 1);
            free(file);
            file = NULL;
            file = (char*) malloc(sizeof(char));
            if (file == NULL)
            {
                free(file);
                fprintf(stderr, "Memory allocation error\n");
                exit(1);
            }
            j = 0;
        }
    }
    
    if (j > 0)
    {
        translateFile(file, from, to, j);
    }
    free(file);
    exit(0);
}
