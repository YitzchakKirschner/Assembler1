#include <stdio.h>
#include <string.h>
#include "errors.h"

int main(int argc, char* argv[]){
    int i; /* Counter */

    /* Verify recipe of file name in argv */
    if(argc < 2){
        error_output(1);
        return -1;
    }

    /* Open the files one by one and verify the given file exists in folder */
    for(i=1; i<argc; i++){
        FILE* as_file;
        strcat(*++argv, ".as"); /* Add "as" to the end of the file name and increment the argv ptr*/
        as_file = fopen(*argv, "r");

        /* Confirm file has opened correctly */
        if (as_file == NULL){
            error_output(2);
            return -1;
        }
        /* Close the .as file */
        fclose(as_file);
    }
    return 1;
}