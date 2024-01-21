#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_extracter.h"
#include "errors.h"
/* macro to check if first num numbers in ptr, are str*/
#define firstWord(ptr, str, num) (strncmp((ptr), (str), (num)) == 0)

FILE* extractMacros(FILE* as_file_ptr, char* file_name){
    FILE *am_file_ptr;
    char am_file_name[MAX_FILE_NAME_LENGTH];
    char line[MAX_LINE_LENGTH];
    MacroNode *macros = NULL; /*List of macros*/
    int in_macro_flag = 0;
    MacroNode *foundMacro = NULL;
    MacroNode *current_macro = NULL;
    MacroNode *temp = NULL;
    char first_field[MAX_MACRO_NAME_LENGTH];
    int i; /*counter*/

    /* Get file name and open file. */
    strcpy(am_file_name, file_name);
    strcat(am_file_name, ".am"); /* Add "as" to the end of the file name and increment the argv ptr*/
    am_file_ptr = fopen(am_file_name, "w");

    /* Confirm file opening. */
    if (!am_file_ptr){
            error_output(2);
            fclose(as_file_ptr);
            return NULL;
        }

    /* Loop line by line. if end or begining of macro, change flag.
       If the first word if the file is the name of a macro, replace with macro.
       If the line is not macro related then copy to output file. */
    while (fgets(line, MAX_LINE_LENGTH, as_file_ptr)) {
        if (in_macro_flag) {
            if (isMcrOrEndmcr(line)) {/*Check for endmcr*/
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
            /* Replace macro with its lines*/
            for (i = 0; i < foundMacro->line_count; i++) {
                fputs(foundMacro->lines[i], am_file_ptr);
            }
        } else if (isMcrOrEndmcr(line)) {/*Macro definition*/
            in_macro_flag = 1;
            current_macro = addMacro(&macros, line + 4); /* line + 4 skips "mcr " */
        } else {/* just a normal line */
            fputs(line, am_file_ptr);
        }
    }

    /* Free the linked list */
    while (macros) {
        temp = macros;
        macros = macros->next;
        free(temp);
    }

    return am_file_ptr;
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
MacroNode* findMacro(MacroNode *head, const char *name) {
    while (head) {
        if (strcmp(head->name, name) == 0) {
            return head;
        }
        head = head->next;/* Search next MacroNode in list*/
    }
    return NULL;/* Not a macro name */
}

/* adds the line to the macro and updates line_count */
void addLineToMacro(MacroNode *macro, const char *line) {
    if (macro->line_count < MAX_MACRO_LINES) {
        strncpy(macro->lines[macro->line_count++], line, MAX_LINE_LENGTH);
    }
}


MacroNode* addMacro(MacroNode **head, const char *name) {
    MacroNode *newNode = (MacroNode *)malloc(sizeof(MacroNode));
    if (newNode) {
        strncpy(newNode->name, name, MAX_MACRO_NAME_LENGTH);
        newNode->line_count = 0;
        newNode->next = *head;
        *head = newNode;
    }
    return newNode;
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
