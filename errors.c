#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include <errno.h>

void error_output(int error_code){
    switch (error_code)
    {
    case NO_PATH:
        fprintf(stderr, "You've not entered a file path\n");
        break;

    case FILE_NOT_OPENED:
        fprintf(stderr, "The file you've given does not exist in the folder :(\n");
        break;

    case USED_SAVED_WORD:
        fprintf(stderr, "The word you used to name your macro, is a saved word. Try using a different name.\n");
        break;
        
    case FAILED_TO_ALLOCATE_MEMORY:
        fprintf(stderr, "The memory allocation has failed, try correcting your malloc function");
        break;

    default:
        break;
    }
}