#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macro_extracter.h"
#include "errors.h"

FILE* getAmFile(FILE* as_file_ptr, char* file_name){
    FILE *am_file_ptr;
    char am_file_name[MAX_FILE_NAME_LENGTH];
    int extraction_complete;

    /* Get file name and open file. */
    strcpy(am_file_name, file_name);
    strcat(am_file_name, ".am"); /* Add "as" to the end of the file name and increment the argv ptr*/
    am_file_ptr = fopen(am_file_name, "w+");

    /* Confirm file opening. */
    if (!am_file_ptr){
            error_output(2);
            fclose(as_file_ptr);
            return NULL;
        }

    extraction_complete = extractMacros(as_file_ptr, am_file_ptr);

    if(!extraction_complete)
        return NULL;

    return am_file_ptr;
}
    /* Loop line by line. if end or begining of macro, change flag.
       If the first word if the file is the name of a macro, replace with macro.
       If the line is not macro related then copy to output file. */
int extractMacros(FILE* as_file_ptr, FILE* am_file_ptr){
    char line[MAX_LINE_LENGTH_PLUS_1 + 1];
    MacroNode *macros = NULL; /*List of macros*/
    int in_macro_flag = 0;
    MacroNode *foundMacro = NULL;
    MacroNode *current_macro = NULL;
    char first_field[MAX_MACRO_NAME_LENGTH];
    int i; /*counter*/
    Line *currentLine = NULL;

    while (fgets(line, MAX_LINE_LENGTH_PLUS_1 + 1, as_file_ptr)) {
        /* Check if the line is longer then 81 chars (includes \n)*/
        if(!validLineLength(line)){
            error_output(5);
            return 0;
        }

        if(isSemicolon(line)){/* Is the line a comment line?*/
            continue; /* Go to next line*/
        }

        if (in_macro_flag) {
            if (isEndmcr(line)) {/*Check for endmcr*/
                in_macro_flag = 0;
                continue; /* No need to add to macro .am file */
            } else {    
                addLineToMacro(current_macro, line);
                continue; /* No need to add to macro .am file */
            }
        }

	    /* Store the first word in the line into first_field, skip any amount of spaces or tabs*/
        getFirstWord(line, first_field);

	    /* If the first word in the line is a name of a macro, replace it with the macro */
        foundMacro = findMacro(macros, first_field);
        if (foundMacro) {
            /* Replace macro with its lines */
            currentLine = foundMacro->lines;
            while (currentLine != NULL) {
                fputs(currentLine->text, am_file_ptr);
                currentLine = currentLine->next;
            } 
        }else if (isMcr(line)) {/*Macro definition*/
            in_macro_flag = 1;
            getFirstWord(line + 4, first_field); /* set first_field to line + 4, skips "mcr " */
            if(isSavedWord(first_field)){ /* Used a saved word to name a macro */
                error_output(3);
                return 0;
            }
            current_macro = addMacro(&macros, first_field); /* Add the macro with the name of first_field*/
        } else {/* just a normal line, add to am file*/
            fputs(line, am_file_ptr);
        }
    }
    /* Free the linked list */
    freeMacros(macros);
    return 1;
}

/*The function returns true if first word of line is "mcr" or "endmcr"*/
int isMcrOrEndmcr (const char* line){
    int i = 0;
    int mcr;
    int endmcr;
    /* Skip leading spaces and tabs*/
    while (line[i] == ' ' || line[i] == '\t')
        i++;

    /* Check if the rest of the line starts with "mcr " */
    mcr = (firstWord(line+i, "mcr ", 4)) || (firstWord(line+i, "mcr\t", 4)) || (firstWord(line+i, "mcr\n", 4));
    endmcr = (firstWord(line+i, "endmcr ", 7)) || (firstWord(line+i, "endmcr\t", 7)) || (firstWord(line+i, "endmcr\n", 7));
    return mcr || endmcr;
}

/* returns the macro of the first word in the line if exsists */
MacroNode* findMacro(MacroNode *macroHead, const char *name) {
    while (macroHead) {
        if (!strcmp(macroHead->name, name)) {
            return macroHead;
        }
        macroHead = macroHead->next;/* Search next MacroNode in list*/
    }
    return NULL;/* Not a macro name */
}

/* adds the line to the macro and updates line_count */

void addLineToMacro(MacroNode *macro, const char *line) {
    Line *newLine = (Line *)malloc(sizeof(Line));
    if (!newLine) { /* Handle memory allocation error*/
        error_output(4);
        return;
    }
    strncpy(newLine->text, line, MAX_LINE_LENGTH_PLUS_1);
    newLine->next = NULL;

    if (macro->line_count == 0) {
        macro->lines = newLine;
    } else {
        Line *current = macro->lines;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newLine;
    }
    macro->line_count++;
}


MacroNode* addMacro(MacroNode **head, const char *name) {
    MacroNode *newNode = (MacroNode *)malloc(sizeof(MacroNode));
    if (newNode) {
        strncpy(newNode->name, name, MAX_MACRO_NAME_LENGTH);
        newNode->line_count = 0;
        newNode->next = *head;
        *head = newNode;
        return newNode;
    }
    error_output(4);/* Failed to allocate memory */
    return NULL;
}


void getFirstWord(const char *line, char *firstWord) {
    int i = 0, j = 0;

    /* Skip leading whitespaces */
    while (line[i] == ' ' || line[i] == '\t') {
        i++;
    }

    /* Capture the first word*/
    while (line[i] != '\0' && line[i] != ' ' && line[i] != '\t' && line[i] != '\n') {
        firstWord[j++] = line[i++];
    }

    /* Null-terminate the first word */
    firstWord[j] = '\0';
}

/* wrap isMcrOrEndmcr for readability*/
int isMcr(const char* line){
    return isMcrOrEndmcr(line);
}

/* wrap isMcrOrEndmcr for readability*/
int isEndmcr(const char* line){
    return isMcrOrEndmcr(line);
}

void freeMacros(MacroNode *macros){
    MacroNode *tempMacro;
    while (macros != NULL) {
        Line *line = macros->lines;
        while (line != NULL) {
            Line *tempLine = line;
            line = line->next;
            free(tempLine);
        }
        tempMacro = macros;
        macros = macros->next;
        free(tempMacro);
    }
}

int validLineLength(char line[]){
    int i;
    for(i = 0; i < MAX_LINE_LENGTH_PLUS_1; i++){
        if(line[i] == '\0')
            return 1;
    }
    return 0;
}

int isSemicolon(const char *line) {
    /* Iterate through each character in the string */
    while (*line != '\0') {
        if (!isspace((unsigned char)*line)) {  /* Check if the character is not a whitespace*/
            return *line == ';';  /* Return true if it's a semicolon, false otherwise */
        }
        line++;  /* Move to the next character */
    }
    return 1;  /* Return true if only whitespace characters are found. unnecessary line */
}