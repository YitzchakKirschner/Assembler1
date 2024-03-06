#include "saved_words.h"
#define MAX_LINE_LENGTH_PLUS_1 82
#define MAX_MACRO_NAME_LENGTH 80
#define MAX_MACRO_LINES 100
#define MAX_FILE_NAME_LENGTH 100
/* macro to check if first num numbers in ptr, are str*/
#define firstWord(ptr, str, num) (strncmp((ptr), (str), (num)) == 0)

/* Line struct definition */
typedef struct Line {
    char text[MAX_LINE_LENGTH_PLUS_1];
    struct Line *next;
} Line;

/* Macro linked list struct */
typedef struct MacroNode {
    char name[MAX_MACRO_NAME_LENGTH];
    Line *lines;  /* Pointer to the first line*/
    int line_count;
    struct MacroNode *next;
} MacroNode;


/* Prototype functions */
FILE* getAmFile(FILE* as_file_ptr, char* file_name, MacroNode** macros);
int extractMacros(FILE* as_file_ptr, FILE* am_file_ptr, MacroNode** macros);
int isMcrOrEndmcr(const char *line);
MacroNode* findMacro(MacroNode *head, const char *name);
void addLineToMacro(MacroNode *macro, const char *line);
MacroNode** addMacro(MacroNode** head, const char *name);
void getFirstWord(const char *line, char *firstWord);
int isMcr(const char* line);
int isEndmcr(const char* line);
void freeMacros(MacroNode *macros);
int validLineLength(char line[]);
int isSemicolon(const char *line);

