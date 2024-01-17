#include "macro_extracter.h"

FILE* extractMacros(FILE* as_file_ptr, char* file_name){
    FILE *am_file;
    char am_file_name[MAX_FILE_NAME_LENGTH];
    char line[MAX_LINE_LENGTH];
    Macro macros[MAX_MACRO_LINES];
    int macro_count = 0;
    int in_macro = 0;
    Macro *currentMacro = NULL;

    /* Open output file and confirm opening */
    strcpy(am_file_name, file_name);
    strcat(*am_file_name, ".am"); /* Add "as" to the end of the file name and increment the argv ptr*/
    am_file = fopen(*am_file_name, "w");
}