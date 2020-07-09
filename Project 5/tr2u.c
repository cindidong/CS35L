#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

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
        k++;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3)
    {
        write(2, "There needs to be 3 operands\n", 29);
        exit(1);
    }
    char* from = argv[1];
    char* to = argv[2];
    size_t fromSize = strlen(from);
    size_t toSize = strlen(to);
    if (fromSize != toSize)
    {
        write(2, "Operands need to be the same length\n", 36);
        exit(1);
    }
    //searching for duplicates
    int table[255];
    size_t i;
    for (i = 0; from[i] != '\0'; i++) {
        if (table[(int)from[i]] == 1)
        {
            write(2, "There are duplicates\n", 21);
            exit(1);
        }
        table[(int)from[i]] = 1;
    }
    
    char* file = (char*) malloc(sizeof(char));
    char c[1];
    int j = 0;
    const int buffSize = 10000;
    //STDIN_FILENO?????
    int readStatus = 1;
    int writeStatus;
    //abc ab a
    while (readStatus != 0) {
        int readStatus = read(0, c, 1);
        if (readStatus < 0)
        {
            write(2, "Input error\n", 12);
            exit(1);
        }
        if (readStatus == 0)
            break;
        if (j < buffSize) {
            char* temp = realloc(file, (j+1)*sizeof(char));
            if (temp == NULL) {
                free(file);
                write(2, "Memory allocation error\n", 24);
                exit(1);
            }
            file = temp;
            file[j] = c[0];
            j++;
        }
        //make space in memory in the case of large files
        else
        {
            char* temp = realloc(file, (j+1)*sizeof(char));
            if (temp == NULL)
            {
                free(file);
                write(2, "Memory allocation error\n", 24);
                exit(1);
            }
            file = temp;
            file[j] = c[0];
            j++;
            translateFile(file, from, to, buffSize + 1);
            writeStatus = write(1, file, j);
            if (writeStatus < 0)
            {
                write(2, "Output error\n", 13);
                exit(1);
            }
            free(file);
            file = NULL;
            file = (char*) malloc(sizeof(char));
            if (file == NULL)
            {
                free(file);
                write(2, "Memory allocation error\n", 24);
                exit(1);
            }
            j = 0;
        }
    }
    //check value of j
    if (j > 0)
    {
        translateFile(file, from, to, j);
        writeStatus = write(1, file, j);
        if (writeStatus < 0)
        {
            write(2, "Output error\n", 13);
            exit(1);
        }
    }
    free(file);
    exit(0);
}
