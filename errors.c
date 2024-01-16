#include <stdio.h>
#include <errno.h>
#include "errors.h"

void error_output(int error_code){
    switch (error_code)
    {
    case NO_PATH:
        perror("You've not entered a file path\n");
        exit(EXIT_FAILURE);
        break;
        
    case FILE_NOT_OPENED:
        perror("The file you've given does not exist in the folder :(\n");
        exit(EXIT_FAILURE);
        break;

    default:
        break;
    }
}