#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 1024
#define MAX_MACRO_NAME_LENGTH 100
#define MAX_MACRO_LINES 100
#define MAX_FILE_NAME_LENGTH 100

typedef struct {
    char name[MAX_MACRO_NAME_LENGTH];
    char lines[MAX_MACRO_LINES][MAX_LINE_LENGTH];
    int line_count;
} Macro;

/* Prototype functions */
int isMacroDefinition(const char *line);
int isEndMacroDefinition(const char *line);
int findMacro(Macro *macros, int macro_count, const char *name);
void addLineToMacro(Macro *macro, const char *line);
FILE* extractMacros(FILE* as_file, char* file_name);