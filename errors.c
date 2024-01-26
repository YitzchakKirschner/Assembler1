#include <stdio.h>
#include <stdlib.h>
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

    case USED_SAVED_WORD:
        printf("The word you used to name your macro, is a seved word. Try using a different name.\n");
        break;
        
    case FAILED_TO_ALLOCATE_MEMORY:
        printf("The memory allocation has failed, try correcting your malloc function");
        break;

    default:
        break;
    }
}