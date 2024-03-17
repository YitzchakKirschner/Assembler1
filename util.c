#include <stdio.h>

int countLinesInFile(FILE* file) {
    int count = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }

    // Reset file pointer to the beginning of the file
    rewind(file);

    return count;
}