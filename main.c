#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_extracter.h"
#include "errors.h"
#include "first_run.h"



int main(int argc, char* argv[]){
    int i; /* Counter */
    FILE* as_file_ptr;
    FILE* am_file_ptr;
    
    FILE* output_file_ptr;
    MacroNode **macro_head = (MacroNode **)malloc(sizeof(MacroNode)); /*List of macros*/
    Symbol **symbolTable = (Symbol **)malloc(sizeof(Symbol));
    if(!macro_head || !symbolTable){
        error_output(3);
        return 0;
    }
    int IC; /* Instruction Count */
    int DC; /* Data Count */
    char as_file_name[MAX_FILE_NAME_LENGTH];
    defineLanguage(); /* Create our insturction set*/

    /* Verify recipe of file name in argv */
    if(argc < 2){
        error_output(1);
        freeLanguage(head);
        return 0;
    }

    /* Open the files one by one and verify the given file exists in folder */
    for(i=1; i<argc; i++){
        IC = 0;
        DC = 0;

        strcpy(as_file_name, argv[i]);
        strcat(as_file_name, ".as"); /* Add "as" to the end of the file name and increment the argv ptr*/
        as_file_ptr = fopen(as_file_name, "r");

        /* Confirm file has opened correctly */
        if (!as_file_ptr){
            error_output(2);
            fclose(as_file_ptr);
            freeLanguage(head);
            return 0;
        }

        /* Pointer to file after macro extraction */
        am_file_ptr = getAmFile(as_file_ptr, argv[i], macro_head);
        if (!am_file_ptr){/* Error, no file returned*/
            error_output(2);
            fclose(as_file_ptr);
            freeLanguage(head);
            return 0;
        }

        output_file_ptr = firstRun(am_file_ptr, argv[i], symbolTable, *macro_head, IC, DC);
        if (!output_file_ptr){/* Error, no file returned*/
            error_output(2);
            fclose(output_file_ptr);
            freeSymbolTable(*symbolTable);
            return 0;
        }
        /* Close the .as and .am files */
        fclose(as_file_ptr);
        fclose(am_file_ptr);
        fclose(output_file_ptr);
        /* Free the linked list */
        freeMacros(*macro_head);
        freeSymbolTable(*symbolTable);
    }

    freeLanguage(head);

    return 0;
}