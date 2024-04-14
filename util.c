#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "util.h"


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

char* removeBracketedNumber(char *str, int *number, Symbol** symbolTable) {
    int length = strlen(str);
    if (length < 3) { // Minimum length for "[0]" to be possible
        *number = -1; // Indicate no number found
        return NULL;
    }

    // Pointer to the last character
    char *end = str + length - 1;

    // Check if the string ends with ']'
    if (*end != ']') {
        *number = -1;
        return NULL;
    }

    // Find the corresponding opening '['
    char *start = strrchr(str, '[');

    // Ensure '[' exists and is before the ']'
    if (!start || start >= end - 1) {
        *number = -1;
        return NULL;
    }

    // Check that all characters between '[' and ']' are digits
    for (char *p = start + 1; p < end; p++) {
        if (!isdigit(*p)) {
            Symbol* found_symbol = findSymbol(*symbolTable, p, MDEFINE);
    
            if (found_symbol && found_symbol->type == MDEFINE) {
                *number = found_symbol->value; // Replace the name with its value
            } else {
                *number = -1;
                return NULL;
            }
        }
    }
    

    // Extract the number
    char *numStr = strndup(start + 1, end - start - 1);
    *number = atoi(numStr);
    free(numStr);

    // Create a new string excluding the '[number]'
    char *newStr = malloc(start - str + 1);
    if (!newStr) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    strncpy(newStr, str, start - str);
    newStr[start - str] = '\0';

    return newStr;
}


Symbol* findSymbol(Symbol* symbolTable, char* name, int type){
    Symbol* current_symbol = symbolTable;
    while(current_symbol){
        if(!strcmp(current_symbol->name, name) && ((current_symbol->type == type) || type == 5)){
            return current_symbol;
        }
        current_symbol = current_symbol->next;
    }
    return NULL;
}