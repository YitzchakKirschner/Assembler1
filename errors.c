#include <stdio.h>
#include "errors.h"

void error_output(int error_code){
    switch (error_code)
    {
    case NO_PATH:
        printf("You've not entered a file path\n");
        break;

    case FILE_NOT_OPENED:
        printf("The file you've given does not exist in the folder :(\n");
        break;

    default:
        break;
    }
}