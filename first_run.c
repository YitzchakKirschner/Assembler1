
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macro_extracter.h"
#include "errors.h"
#include "first_run.h"




#define MAX_TAG_LENGTH 31

/* Main Assembler Algorithm Functions */
FILE* firstRun(FILE* am_file_ptr, char* file_name, Symbol **symbolTable, MacroNode* macro_head, int IC, int DC){
    FILE *output_file_ptr;
    FILE* object_file_ptr;
    //int file_length = countLinesInFile(am_file_ptr);
    //int decoded_lines[file_length];

    OutputLines* data_output_head = (OutputLines*)malloc(sizeof(OutputLines));
    if (!data_output_head) {
        error_output(8); // Handle memory allocation failure
        return NULL; // Return NULL or handle the error as per your requirement
    }
    OutputLines** data_output = (OutputLines**)malloc(sizeof(OutputLines));
    *data_output = data_output_head;

    OutputLines* instruction_output_head = (OutputLines*)malloc(sizeof(OutputLines));
    if (!instruction_output_head) {
        error_output(8); // Handle memory allocation failure
        return NULL; // Return NULL or handle the error as per your requirement
    }
    OutputLines** instruction_output = (OutputLines**)malloc(sizeof(OutputLines));
    *instruction_output = instruction_output_head;

    char object_file_name[MAX_FILE_NAME_LENGTH];
    char output_file_name[MAX_FILE_NAME_LENGTH];
    char line[MAX_LINE_LENGTH_PLUS_1], first_word[MAX_MACRO_NAME_LENGTH], second_word[MAX_MACRO_NAME_LENGTH];
    int src_line = 0;
    int tag_flag = 0;
   

    /* Get file name and open file. */
    strcpy(output_file_name, file_name);
    strcat(output_file_name, ".output"); /* Add "output" to the end of the file name */
    output_file_ptr = fopen(output_file_name, "w+");


    /* Confirm file opening. */
    if (!output_file_ptr){
        error_output(2);
        return NULL;
    }

    strcpy(object_file_name, file_name);
    strcat(object_file_name, ".ob"); /* Add "output" to the end of the file name */
    object_file_ptr = fopen(object_file_name, "w+");

    if (!object_file_ptr){
        error_output(2);
        return NULL;
    }

    rewind(am_file_ptr);

    while (fgets(line, MAX_LINE_LENGTH_PLUS_1, am_file_ptr)) {
        /* Store the first word in the line into first_field, skip any amount of spaces or tabs*/
        getWordAtIndex(line, first_word, 1);
        if (first_word[0] == '\0') 
            return output_file_ptr; /* End of file */

        if (strcmp(first_word, ".define") == 0) {
            processDefineStatement(line, symbolTable, macro_head);
        } else {
        
            if (isTag(first_word, macro_head)) {
                tag_flag = 1;
            } else {
                tag_flag = 0;
            }

            /*make second word the first not-tag-word*/
            if(!tag_flag){ 
                strcpy(second_word, first_word);
            } else {
                getWordAtIndex(line, second_word, 2);
            }

            if(strcmp(second_word, ".data") == 0){
                processDataDirective(line, symbolTable, &DC, &src_line, DATA_CODE, first_word, tag_flag, data_output);
            } else if(strcmp(second_word, ".string") == 0){
                processDataDirective(line, symbolTable, &DC, &src_line, STRING_CODE, first_word, tag_flag, data_output);
            } /*else if(strcmp(second_word, ".entry") == 0){
                processCodeDirectives(line, symbolTable, &IC);
            } else if(strcmp(second_word, ".extern") == 0){
                processCodeDirectives(line, symbolTable, &IC);
            } else {
                processCodeDirectives(line, symbolTable, &IC);
            }*/

            else
                processCodeDirective(line, symbolTable, &IC, &src_line, first_word, tag_flag, instruction_output, &data_output_head);
        }
        src_line++;

    }
    //writeDataToFile(data_output_head, output_file_ptr);

    freeOutputLines(data_output_head);
    freeOutputLines(instruction_output_head);

    return output_file_ptr;
}


/* Function to Process 'define' Statements */
void processDefineStatement(char *line, Symbol **symbolTable, MacroNode* macro_head) {
    char name[MAX_MACRO_NAME_LENGTH];
    int value;
    Symbol** current_symbol = symbolTable;
    // Extract the name and value from the line
    if(sscanf(line, ".define %s = %d", name, &value) != 2)
        error_output(6);
    // Insert into symbol table with MDEFINE property
    /* check the name doesn't appear in the symbol table yet */
    while(*current_symbol){
        if(findSymbol(*symbolTable, name, 5)){
            error_output(7);
        }
        current_symbol = &(*current_symbol)->next;
    }

    if(findMacro(macro_head, name))
        error_output(9);

    insertIntoSymbolTable(current_symbol, name, value, MDEFINE);
}



void processDataDirective(char *line, Symbol **symbolTable, int *DC, int* src_line, int data_type, char* first_word, int tag_flag, OutputLines** data_output) {
    char command_name[MAX_MACRO_NAME_LENGTH];
    Symbol** current_symbol = symbolTable;
    if (tag_flag) {
        // Check and remove the trailing colon from first_word
        size_t len = strlen(first_word);
        if (len > 0 && first_word[len - 1] == ':') {
            first_word[len - 1] = '\0';  // Remove the trailing colon by setting it to null terminator
        }

        // Insert into symbol table with DATA property
        /* check the name doesn't appear in the symbol table yet */
        while (*current_symbol) {
            if (!strcmp((*current_symbol)->name, first_word)) {
                error_output(7); // Assuming error_output(7) handles duplicate symbol names
                return;  // Optional: return from function if error occurs
            }
            current_symbol = &((*current_symbol)->next);
        }
        insertIntoSymbolTable(current_symbol, first_word, *DC, DATA);
    }


    if(data_type == DATA_CODE){  
        writeBinaryNumbersToDataSegment(data_output, strstr(line, ".data") + 5, DC);
    }
    else if(data_type == STRING_CODE){
        writeAsciiBinaryToDataSegment(data_output, strstr(line, ".string") + 7, DC);
    }
}

/* Helper Function to Insert into Symbol Table */
void insertIntoSymbolTable(Symbol **current_symbol, char *name, int value, int type) {

    // Create a new symbol
    *current_symbol = (Symbol *)malloc(sizeof(Symbol));
    if (!(*current_symbol)) {
        error_output(FAILED_TO_ALLOCATE_MEMORY);
    }
    strcpy((*current_symbol)->name, name);
    (*current_symbol)->value = value;
    (*current_symbol)->type = type;
    (*current_symbol)->next = NULL;
}

int isTag(char* word, MacroNode* macros){
    int len = strlen(word);
    char tag[len];
    int i;

    for(i = 0; i < len -1; i++){
        tag[i] = word[i];
    }
    tag[i] = '\0';

    // Check if word length is less than 32
    if (len >= 33 || len <= 1) {
        return 0;
    }

    // Check if first character is alphabetic
    if (!isalpha(word[0])) {
        return 0;
    }

    // Check if last character is ':'
    if (word[len - 1] != ':') {
        return 0;
    }

    if (findMacro(macros, tag) || isSavedWord(tag)){
        error_output(8);
    }


    // Check if remaining characters are alphanumeric
    for (int i = 1; i < len -1; i++) {
        if (!isalnum(word[i])) {
            return 0;
        }
    }

    return 1;
}

void writeBinaryNumbersToDataSegment(OutputLines** data_output, const char* numbers, int* DC) {
    OutputLines* current = *data_output;  // Create a temporary pointer to traverse the list
    char tempNumbers[82];  // Temporary numbers buffer
    char* token;
    


    // Copy numbers to tempNumbers so strtok doesn't modify original string
    strcpy(tempNumbers, numbers);

    // Get the first token
    token = strtok(tempNumbers, ",");

    // Walk through other tokens
    while (token != NULL) {

        OutputText *newOutputText = (OutputText *)malloc(sizeof(OutputText));
        if (!newOutputText) { /* Handle memory allocation error*/
            error_output(4);
            return;
        }
        newOutputText->next = NULL;

        int number = atoi(token);  // Convert string to integer
        char binary[16];  // Buffer to hold binary number

        // Convert number to binary and store in binary buffer
        for (int i = 13; i >= 0; i--) {
            binary[i] = (number & 1) + '0';
            number >>= 1;
        }
        binary[14] = '\n';  // Null-terminate the binary string
        binary[15] = '\0';  // Null-terminate the binary string

        newOutputText->output_line_number = *DC;
        strcpy(newOutputText->text, binary);
        current->firstLine = newOutputText;
        current->is_decoded = 1;
        current->next = (OutputLines*)malloc(sizeof(OutputLines));
        if (!current->next) {
            error_output(FAILED_TO_ALLOCATE_MEMORY);
        }
        current = current->next;  // Move to the next node

        // Increment the data counter
        (*DC)++;

        token = strtok(NULL, ",");

    }
    *data_output = current;
}

void writeAsciiBinaryToDataSegment(OutputLines** data_output, const char* str, int* DC) {
    OutputLines* current = *data_output;  // Create a temporary pointer to traverse the list
    char tempStr[82];  // Temporary string buffer
    char* token;
    int insideQuotes = 0;  // Flag to track whether we're inside quotes

    // Copy str to tempStr so strtok doesn't modify original string
    strcpy(tempStr, str);

    // Get the first token
    token = strtok(tempStr, "\"");

    // Walk through other tokens
    while (token != NULL) {
        if (insideQuotes) {
            for (int j = 0; j < strlen(token); j++) {

                OutputText *newOutputText = (OutputText *)malloc(sizeof(OutputText));
                if (!newOutputText) { /* Handle memory allocation error*/
                    error_output(4);
                    return;
                }
                newOutputText->next = NULL;
                
                int asciiVal = token[j];  // Get ASCII value of character
                char binary[16];  // Buffer to hold binary number
                // Convert ASCII value to binary and store in binary buffer
                for (int i = 13; i >= 0; i--) {
                    binary[i] = (asciiVal & 1) + '0';
                    asciiVal >>= 1;
                }
                binary[14] = '\n';  // Null-terminate the binary string
                binary[15] = '\0';  // Null-terminate the binary string

                newOutputText->output_line_number = *DC;
                current->is_decoded = 1;
                strcpy(newOutputText->text, binary);
                current->firstLine = newOutputText;

                current->next = (OutputLines*)malloc(sizeof(OutputLines));
                if (!current->next) {
                    error_output(FAILED_TO_ALLOCATE_MEMORY);
                }
                current = current->next;  // Move to the next node

                // Increment the data counter
                (*DC)++;
            }
        }

        // Flip the insideQuotes flag
        insideQuotes = !insideQuotes;

        token = strtok(NULL, "\"");
    }
    *data_output = current;
}


void freeSymbolTable(Symbol *symbolTable) {
    if (symbolTable == NULL) {
        return;  // Symbol table is already empty
    }

    Symbol *tempSymbol;
    while (symbolTable != NULL) {
        tempSymbol = symbolTable;
        symbolTable = symbolTable->next;  // Update head pointer correctly
        free(tempSymbol);
    }
}

void freeOutputLines(OutputLines *outputLines) {
    if (outputLines == NULL) {
        return;  // Output lines are already empty
    }

    OutputLines *tempOutputLine;
    while (outputLines != NULL) {
        tempOutputLine = outputLines;
        outputLines = outputLines->next;  // Update head pointer correctly
        free(tempOutputLine);
    }
}



void processCodeDirective(char *line, Symbol **symbolTable, int *IC, int* src_line, char* first_word, int tag_flag, OutputLines** instruction_output, OutputLines** data_output){
    Word* current_word = (Word*)malloc(sizeof(Word));
    if (!current_word) { /* Handle memory allocation error*/
        error_output(4);
        return;
    }
    current_word = head;
    while(current_word){
        if(strcmp(first_word, current_word->name) == 0){
            processInstruction(first_word, line, symbolTable, IC, src_line, instruction_output, data_output);
        }
        current_word = current_word->next;
    }
}

void processInstruction(char* instruction, char* line, Symbol** symbolTable, int* IC, int* src_line, OutputLines** instruction_output, OutputLines** data_output){
    OutputLines* current = *instruction_output;  // Create a temporary pointer to traverse the list
    Symbol* found_symbol;
    char tempLine[82];  // Temporary numbers buffer

    char src_word[MAX_MACRO_NAME_LENGTH];
    char dst_word[MAX_MACRO_NAME_LENGTH];
    char word[MAX_MACRO_NAME_LENGTH];

    char src_ARE[3];
    char dst_ARE[3];
    char ARE[3];

    char* src_bin_word;
    char* dst_bin_word;
    char* bin_word;

    int srcExtractedNumber = -1;
    int dstExtractedNumber = -1;
    int extractedNumber;

    char* token;
    char* constArr;
    int i;

    OutputText *newOutputText = (OutputText *)malloc(sizeof(OutputText));
    if (!newOutputText) { /* Handle memory allocation error*/
        error_output(4);
        return;
    }
    newOutputText->next = NULL; 
    current->firstLine = newOutputText;

    strcpy(current->opcode, "0000");

    // Copy numbers to tempNumbers so strtok doesn't modify original string
    strcpy(tempLine, line);

    getWordAtIndex(tempLine, src_word, 2);
    getWordAtIndex(tempLine, dst_word, 3);
    token = strtok(tempLine, ", \t");

    for(i = 0; i < 2; i++){ 
        if(i == 0){
            strcpy(word, src_word);
            strcpy(ARE, src_ARE);
            extractedNumber = srcExtractedNumber;
            // strcpy(bin_word, src_bin_word);
        } else {
            strcpy(word, dst_word);
            strcpy(ARE, dst_ARE);
            extractedNumber = dstExtractedNumber;
            // strcpy(bin_word, dst_bin_word);
        }
        
        decodeInstruction(instruction,  symbolTable, current, data_output, word, ARE, &extractedNumber, &bin_word, tempLine, found_symbol, token, i, constArr);

        if (i == 0){
            strcpy(src_ARE, ARE);
            src_bin_word = (char *)malloc(13);
            if (!src_bin_word) { /* Handle memory allocation error*/
                error_output(4);
                return;
            }
            strcpy(src_bin_word, bin_word);
            srcExtractedNumber = extractedNumber;
        } else {
            strcpy(dst_ARE, ARE);
            dst_bin_word = (char *)malloc(13);
            if (!dst_bin_word) { /* Handle memory allocation error*/
                error_output(4);
                return;
            }
            strcpy(dst_bin_word, bin_word);
            dstExtractedNumber = extractedNumber;
        }
    }
    addInstrctionLine(current, newOutputText, IC, src_bin_word, dst_bin_word, src_ARE, dst_ARE, srcExtractedNumber, dstExtractedNumber);
    

    freeInstruction(*instruction_output);
}
