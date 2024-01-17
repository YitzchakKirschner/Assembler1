#include "macro_extracter.h"

FILE* extractMacros(FILE* as_file_ptr, char* file_name){
    FILE *am_file_ptr;
    char am_file_name[MAX_FILE_NAME_LENGTH];
    char line[MAX_LINE_LENGTH];
    MacroNode *macros = NULL;
    int in_macro_flag = 0;
    MacroNode *currentMacro = NULL;

    /* Get file name and open file. */
    strcpy(am_file_name, file_name);
    strcat(am_file_name, ".am"); /* Add "as" to the end of the file name and increment the argv ptr*/
    am_file_ptr = fopen(*am_file_name, "w");

    /* Confirm file opening. */
    if (!am_file_ptr){
            error_output(2);
            fclose(as_file_ptr);
            return NULL;
        }
    
    return am_file_ptr;
}