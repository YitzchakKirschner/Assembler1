#include "saved_words.h"
#define MAX_LINE_LENGTH 1024
#define MAX_MACRO_NAME_LENGTH 100
#define MAX_MACRO_LINES 100
#define MAX_FILE_NAME_LENGTH 100

/* Macro linked list struct */
typedef struct MacroNode {
    char name[MAX_MACRO_NAME_LENGTH];
    char lines[MAX_MACRO_LINES][MAX_LINE_LENGTH];
    int line_count;
    struct MacroNode *next;
} MacroNode;


/* Prototype functions */
FILE* extractMacros(FILE* as_file_ptr, char* file_name, Word* head);
int isMcrOrEndmcr(const char *line);
MacroNode* findMacro(MacroNode *head, const char *name);
void addLineToMacro(MacroNode *macro, const char *line);
MacroNode* addMacro(MacroNode **head, const char *name);
void getFirstWord(const char *line, char *firstWord);
int isMcr(const char* line);
int isEndmcr(const char* line);
