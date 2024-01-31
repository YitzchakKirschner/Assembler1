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

    case EXCEEDED_PERMISSIBLE_LENGTH:
        fprintf(stderr, "At least on of the lines in the input file is longer then 80 charechters");
        break;

    case DEFINE_SYNTAX_ERROR:
        fprintf(stderr, "You've got a syntax error in one of your define statments\nMake sure your format is as follows: .define string = number");
        break;

    case NAME_IS_DUPLICATE:
        fprintf(stderr, "The name you've chosen for one of you tags or define statments is a duplicate");
        break;
        
    default:
        break;
    }
}