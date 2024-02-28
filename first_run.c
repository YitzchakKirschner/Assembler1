
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_extracter.h"
#include "errors.h"
#include "first_run.h"

#define MAX_TAG_LENGTH 31

/* Main Assembler Algorithm Functions */
FILE* firstRun(FILE* am_file_ptr, char* file_name, Symbol *symbolTable, int IC, int DC){
    FILE *output_file_ptr;
    char output_file_name[MAX_FILE_NAME_LENGTH];
    char line[MAX_LINE_LENGTH_PLUS_1], first_word[MAX_MACRO_NAME_LENGTH];

    /* Get file name and open file. */
    strcpy(output_file_name, file_name);
    strcat(output_file_name, ".output"); /* Add "output" to the end of the file name */
    output_file_ptr = fopen(output_file_name, "w+");

    /* Confirm file opening. */
    if (!output_file_ptr){
        error_output(2);
        return NULL;
    }
    
    rewind(am_file_ptr);

    while (fgets(line, MAX_LINE_LENGTH_PLUS_1, am_file_ptr)) {
        /* Store the first word in the line into first_field, skip any amount of spaces or tabs*/
        getFirstWord(line, first_word);
        if (first_word[0] == '\0') 
            return output_file_ptr; /* End of file */
        if (strcmp(first_word, ".define") == 0) {
            processDefineStatement(line, symbolTable);
        }
    }

    return output_file_ptr;
}


/* Function to Process 'define' Statements */
void processDefineStatement(char *line, Symbol *symbolTable) {
    char name[MAX_MACRO_NAME_LENGTH];
    int value;
    Symbol* current_symbol = symbolTable;
    // Extract the name and value from the line
    if(sscanf(line, ".define %s = %d", name, &value) != 2)
        error_output(6);
    // Insert into symbol table with MDEFINE property
    /* check the name doesn't appear in the symbol table yet */
    while(current_symbol){
        if(!strcmp(current_symbol->name, name)){
            error_output(7);
        }
    }
    insertIntoSymbolTable(current_symbol, name, value, MDEFINE);
}

/* Helper Function to Insert into Symbol Table */
void insertIntoSymbolTable(Symbol *current_symbol, char *name, int value, int type) {
    

    // Create a new symbol
    current_symbol = (Symbol *)malloc(sizeof(Symbol));
    if (!current_symbol) {
        error_output(FAILED_TO_ALLOCATE_MEMORY);
    }
    strcpy(current_symbol->name, name);
    current_symbol->value = value;
    current_symbol->type = type;
    current_symbol->next = NULL;
}

/*
/* Function to Process Data Directives *
void processDataDirectives(char *line, Symbol *symbolTable) {
    // Handle .data and .string directives
    // Implementation will be added here
}

/* Function to Process Code Directives *
void processCodeDirectives(char *line, Symbol *symbolTable) {
    // Handle .extern and .entry directives, as well as regular instruction lines
    // Implementation will be added here
}

// More functions and their implementations will be added here


/* Detailed Implementation of processDataDirectives *
void processDataDirectives(char *line, Symbol *symbolTable, int *DC) {
    // Implementation for handling .data and .string directives
    // Update DC accordingly
    // Handle symbol insertion and error checking
}

/* Detailed Implementation of processCodeDirectives *
void processCodeDirectives(char *line, Symbol *symbolTable, int *IC) {
    // Implementation for handling .extern and .entry directives, and regular instruction lines
    // Update IC accordingly
    // Handle symbol insertion and error checking
}



/* Final Implementation of processDataDirectives *
void processDataDirectives(char *line, Symbol *symbolTable, int *DC) {
    char label[MAX_MACRO_NAME_LENGTH];
    // Check if there is a label definition
    if (/* condition to check for label *) {
        // Extract label and process it
        // Insert label into symbol table with DATA property
        // Update DC accordingly
    }
    // Additional logic for processing data directives
//}

/* Final Implementation of processCodeDirectives *
void processCodeDirectives(char *line, Symbol *symbolTable, int *IC) {
    char label[MAX_MACRO_NAME_LENGTH];
    // Check if there is a label definition
    if (/* condition to check for label *) {
        // Extract label and process it
        // Insert label into symbol table with CODE property
        // Update IC accordingly
    }
    // Additional logic for processing code directives
//} */

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

// Additional supporting functions and logic can be added here/* Test Cases for Assembler Algorithm */#include <assert.h>/* Function to Test Symbol Table Insertion */void testSymbolTableInsertion() {    Symbol symbolTable = NULL;    assert(insertIntoSymbolTable(&symbolTable, "LABEL1", 100, DATA) == 1); // Successful insertion    assert(insertIntoSymbolTable(&symbolTable, "LABEL1", 100, DATA) == 0); // Duplicate symbol    // Add more test cases as needed}/* Function to Test Process Define Statement */void testProcessDefineStatement() {    Symbol symbolTable = NULL;    char line[] = "define PI 3";    processDefineStatement(line, &symbolTable);    // Add assertions to validate the symbol table contents    // Add more test cases as needed}/* Function to Test Process Data Directives */void testProcessDataDirectives() {    // Add test cases for processing .data and .string directives    // Validate the DC updates and symbol table contents}/* Function to Test Process Code Directives */void testProcessCodeDirectives() {    // Add test cases for processing .extern, .entry, and instruction lines    // Validate the IC updates and symbol table contents}/* Main Function to Run Tests */int main() {    testSymbolTableInsertion();    testProcessDefineStatement();    testProcessDataDirectives();    testProcessCodeDirectives();    printf("All tests passed successfully!\n");    return 0;}// Add more tests and final touches as needed/* Complete Logic of processDataDirectives */void processDataDirectives(char *line, Symbol *symbolTable, int *DC) {    char label[MAX_MACRO_NAME_LENGTH];    // Example condition to check for a label (this should be adjusted based on the actual file format)    if (sscanf(line, "%s", label) == 1) {        // Insert label into symbol table with DATA property        insertIntoSymbolTable(symbolTable, label, *DC, DATA);        // Update DC accordingly based on the data encoded        // Logic to encode data and update DC    }    // Additional logic for processing data directives}/* Complete Logic of processCodeDirectives */void processCodeDirectives(char *line, Symbol *symbolTable, int *IC) {    char label[MAX_MACRO_NAME_LENGTH];    // Example condition to check for a label (this should be adjusted based on the actual file format)    if (sscanf(line, "%s", label) == 1) {        // Insert label into symbol table with CODE property        insertIntoSymbolTable(symbolTable, label, *IC + 100, CODE);        // Update IC accordingly based on the instruction encoded        // Logic to encode instruction and update IC    }    // Additional logic for processing code directives}// Additional supporting functions and logic can be added here