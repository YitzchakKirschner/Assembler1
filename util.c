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

void toBinary(int number, int n, char *binary, int useTwosComplement) {
    if (useTwosComplement) {
        // Calculate 2's complement for negative numbers
        number = (1 << n) + number;
    }

    for (int i = n - 1; i >= 0; i--) {
        binary[i] = (number & 1) ? '1' : '0';
        number >>= 1;
    }
    binary[n] = '\0'; // Null-terminate the string
}