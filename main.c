#include "macro_extracter.h"
#include "errors.h"

int main(int argc, char* argv[]){
    int i; /* Counter */
    FILE* as_file_ptr;
    FILE* am_file_ptr;
    char as_file_name[MAX_FILE_NAME_LENGTH];

    /* Verify recipe of file name in argv */
    if(argc < 2){
        error_output(1);
        return -1;
    }

    /* Open the files one by one and verify the given file exists in folder */
    for(i=1; i<argc; i++){
        strcpy(as_file_name, argv[i]);
        strcat(as_file_name, ".as"); /* Add "as" to the end of the file name and increment the argv ptr*/
        as_file_ptr = fopen(as_file_name, "r");

        /* Confirm file has opened correctly */
        if (!as_file_ptr){
            error_output(2);
            return -1;
        }

        am_file_ptr = extractMacros(as_file_ptr, argv[i]);

        /* Close the .as file */
        fclose(as_file_ptr);
        fclose(am_file_ptr);
    }
    return 1;
}