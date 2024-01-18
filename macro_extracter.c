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

    while (fgets(line, MAX_LINE_LENGTH, as_file_ptr)) {
        if (in_macro_flag) {
            if (isEndMacroDefinition(line)) {
                in_macro_flag = 0;
            } else {
                addLineToMacro(currentMacro, line);
                continue; /* No need to add to macro file */
            }
        }

	/* Store the first word in the line into first_field*/
        char firstField[MAX_MACRO_NAME_LENGTH];
        sscanf(line, "%s", firstField);

	/* If the first word is a name of a macro, replace it with the macro */
        MacroNode *foundMacro = findMacro(macros, firstField);
        if (foundMacro) {
            // Replace macro with its lines
            for (int i = 0; i < foundMacro->line_count; i++) {
                fputs(foundMacro->lines[i], am_file_ptr);
            }
        } else if (isMacroDefinition(line)) {
            in_macro_flag = 1;
            currentMacro = addMacro(&macros, line + 4); // line + 4 skips "mcr "
        } else {/* just a normal line */
            fputs(line, am_file_ptr);
        }
    }

    return am_file_ptr;
}