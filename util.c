#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "util.h"
#include "errors.h"
#include "saved_words.h"


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

void decodeInstruction(char* instruction, Symbol** symbolTable, OutputLines* current, OutputLines** data_output, char* word, char* ARE, int* extractedNumber, char** bin_word, char* tempLine, Symbol* found_symbol, char* token, int i, char* constArr){
    found_symbol  = findSymbol(*symbolTable, word, 5);

    constArr =  removeBracketedNumber(word, extractedNumber, symbolTable);

    // Get the first token
    token = strtok(NULL, ", \t");

    if (token[0] == '#' && checkRegType(0, instruction, !i)){
        strcpy(ARE, "00");
        if(i == 0)
            strcpy(current->src_operand, "00");
        else
            strcpy(current->dest_operand, "00");    

        (*bin_word) = (char *)malloc(13);
        if (!(*bin_word)) { /* Handle memory allocation error*/
            error_output(4);
            return;
        }

        toBinary(atoi(token + 1), 12, (*bin_word), 1);
    } else if (found_symbol && found_symbol->type == MDEFINE && checkRegType(0, instruction, !i)){
        strcpy(ARE, "00");
        if(i == 0)
            strcpy(current->src_operand, "00");
        else
            strcpy(current->dest_operand, "00");    

        (*bin_word) = (char *)malloc(13);
        if (!(*bin_word)) { /* Handle memory allocation error*/
            error_output(4);
            return;
        }

        toBinary(found_symbol->value, 12, (*bin_word), 1);
    } else if (found_symbol && found_symbol->type == DATA && checkRegType(1, instruction, !i)){
        int textLength;
        (*bin_word) = (char *)malloc(13);
        if (!(*bin_word)) { /* Handle memory allocation error*/
            error_output(4);
            return;
        }

        OutputLines* current_data_output = *data_output; 
        strcpy(ARE, "01");
        if(i == 0)
            strcpy(current->src_operand, "01");
        else
            strcpy(current->dest_operand, "01");    

        while(current_data_output->firstLine->output_line_number != found_symbol->value){
            current_data_output = current_data_output->next;
        }
        textLength = strlen(current_data_output->firstLine->text);
        // Copy the last 12 characters of the text
        strcpy((*bin_word), current_data_output->firstLine->text + (textLength - 13));
        (*bin_word)[12] = '\0';  // Null-terminate the string
    } else if (constArr && (*extractedNumber != -1) && checkRegType(1, instruction, !i)){
        found_symbol = findSymbol(*symbolTable, constArr, DATA);
        if(found_symbol){
            int textLength;
            (*bin_word) = (char *)malloc(13);
            if (!(*bin_word)) { /* Handle memory allocation error*/
                error_output(4);
                return;
            }

            OutputLines* current_data_output = *data_output; 
            strcpy(ARE, "01");
            if(i == 0)
                strcpy(current->src_operand, "10");
            else
                strcpy(current->dest_operand, "10");    

            while(current_data_output->firstLine->output_line_number != (found_symbol->value + *extractedNumber)){
                current_data_output = current_data_output->next;
            }
            textLength = strlen(current_data_output->firstLine->text);
            // Copy the last 12 characters of the text
            strcpy((*bin_word), current_data_output->firstLine->text + (textLength - 13));
            (*bin_word)[12] = '\0';  // Null-terminate the string
        }
    } else if(getRegisterByName(word) && checkRegType(0, instruction, !i)){
        (*bin_word) = (char *)malloc(4);
        if (!(*bin_word)) { /* Handle memory allocation error*/
            error_output(4);
            return;
        }
        strcpy(ARE, "00");
        if(i == 0)
            strcpy(current->src_operand, "11");
        else
            strcpy(current->dest_operand, "11");    
        strcpy((*bin_word), getRegisterByName(word));
    } else if (found_symbol && found_symbol->type == EXTERN && checkRegType(2, instruction, !i)){
        strcpy(ARE, "10");
        if(i == 0)
            strcpy(current->src_operand, "01");
        else
            strcpy(current->dest_operand, "01");    
    } 
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


/*
 * Function to check if the given ARE code is present in the src or dst reg_type
 * fields of the specified command.
 * @param ARE - The addressing mode to check.
 * @param commandName - The name of the command.
 * @param isSrc - If true, check the src_reg_type; if false, check the dst_reg_type.
 * @return true if the ARE code is found in the specified field; false otherwise.
 */
int checkRegType(int ARE, const char* commandName, int isSrc) {
    char ARE_code = '0' + ARE;  // Convert integer ARE to character

    Word* current = head;  // Start at the head of the list
    while (current != NULL) {
        // Compare the command name with the current word name
        if (strcmp(current->name, commandName) == 0) {
            // Check the appropriate register type field based on isSrc
            char* reg_type = isSrc ? current->src_reg_type : current->dst_reg_type;
            if (reg_type != NULL) {
                // Check if the ARE_code is in the string
                if (strchr(reg_type, ARE_code) != NULL) {
                    return 1;  // ARE code is found
                }
            }
            break;  // Exit the loop if the command is found
        }
        current = current->next;  // Move to the next word
    }

    return 0;  // ARE code not found or command not found
}
