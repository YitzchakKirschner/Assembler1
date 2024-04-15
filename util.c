#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "util.h"
#include "errors.h"


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
    char *p = start + 1;

    if (!isdigit(*p)) {
        int pEnd = strlen(p) - 1;
        p[pEnd] = '\0'; // Null-terminate the string
        Symbol* found_symbol = findSymbol(*symbolTable, p, MDEFINE);

        if (found_symbol && found_symbol->type == MDEFINE) {
            *number = found_symbol->value; // Replace the name with its value
        } else {
            *number = -1;
            return NULL;
        }
    } else {
        // Extract the number
        char *numStr = strndup(start + 1, end - start - 1);
        *number = atoi(numStr);
        free(numStr);
    }
    
   

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

void addInstrctionLine(OutputLines* current, OutputText* newOutputText, int* IC, char* src_bin_word, char* dst_bin_word, char* src_ARE, char* dst_ARE, int srcExtractedNumber, int dstExtractedNumber){
    strcpy(&newOutputText->text[0], "0000");
    strcpy(&newOutputText->text[4], current->opcode);
    strcpy(&newOutputText->text[8], current->src_operand);
    strcpy(&newOutputText->text[10], current->dest_operand);
    strcpy(&newOutputText->text[12], "00");
    strcpy(&newOutputText->text[14], "\0");
    newOutputText->output_line_number = *IC;
    (*IC)++;

    newOutputText->next = (OutputText *)malloc(sizeof(OutputText));
    if (!newOutputText->next) { /* Handle memory allocation error*/
        error_output(4);
        return;
    }
    newOutputText = newOutputText->next;

    if((strlen(src_bin_word) > 4) || (strlen(dst_bin_word) > 8)){
        if(strlen(src_bin_word) > 4){
            strcpy(&newOutputText->text[0], src_bin_word);
        } else{
            strcpy(&newOutputText->text[0], "000000");
            strcpy(&newOutputText->text[6], src_bin_word);
            strcpy(&newOutputText->text[9], "00000\0");
        }
        strcpy(&newOutputText->text[12], src_ARE);
        strcpy(&newOutputText->text[14], "\0");
        newOutputText->output_line_number = *IC;
        (*IC)++;
        if(srcExtractedNumber != -1){
            newOutputText->next = (OutputText *)malloc(sizeof(OutputText));
            if (!newOutputText->next) { /* Handle memory allocation error*/
                error_output(4);
                return;
            }
            newOutputText = newOutputText->next;
            toBinary(srcExtractedNumber, 12, newOutputText->text, 0);
            strcpy(&newOutputText->text[12], "00\0"); 
            newOutputText->output_line_number = *IC;
            (*IC)++;
        }

        newOutputText->next = (OutputText *)malloc(sizeof(OutputText));
        if (!newOutputText->next) { /* Handle memory allocation error*/
            error_output(4);
            return;
        }
        newOutputText = newOutputText->next;
        if(strlen(dst_bin_word) > 4){
            strcpy(&newOutputText->text[0], dst_bin_word);
        } else{
            strcpy(&newOutputText->text[0], "000000000");
            strcpy(&newOutputText->text[9], dst_bin_word);
        }
        strcpy(&newOutputText->text[12], dst_ARE);
        strcpy(&newOutputText->text[14], "\0");
        newOutputText->output_line_number = *IC;
        (*IC)++;
        if(dstExtractedNumber != -1){
            newOutputText->next = (OutputText *)malloc(sizeof(OutputText));
            if (!newOutputText->next) { /* Handle memory allocation error*/
                error_output(4);
                return;
            }
            newOutputText = newOutputText->next;
            toBinary(dstExtractedNumber, 12, newOutputText->text, 0);
            strcpy(&newOutputText->text[12], "00\0"); 
            newOutputText->output_line_number = *IC;
            (*IC)++;
        }
    } else {
        strcpy(&newOutputText->text[0], "000000");
        strcpy(&newOutputText->text[6], src_bin_word);
        strcpy(&newOutputText->text[9], dst_bin_word);
        strcpy(&newOutputText->text[12], "00\0");
        newOutputText->output_line_number = *IC;
        (*IC)++;
    }
}

char* getRegisterByName(char* name){
    if(!strcmp(name, "r0")){
        return "000";
    } else if(!strcmp(name, "r1")){
        return "001";
    } else if(!strcmp(name, "r2")){
        return "010";
    } else if(!strcmp(name, "r3")){
        return "011";
    } else if(!strcmp(name, "r4")){
        return "100";
    } else if(!strcmp(name, "r5")){
        return "101";
    } else if(!strcmp(name, "r6")){
        return "110";
    } else if(!strcmp(name, "r7")){
        return "111";
    } else {
        return NULL;
    }
}

void freeInstruction(OutputLines* instruction_output){
    OutputLines* current = instruction_output;
    while(current){
        OutputLines* temp = current;
        current = current->next;
        while (temp->firstLine)
        {
            OutputText* temp2 = temp->firstLine;
            temp->firstLine = temp->firstLine->next;
            free(temp2);
        }
        free(temp);
    }
}