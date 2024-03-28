#include "errors.h"
#include <stdio.h>
#include <stdlib.h>

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
        fprintf(stderr, "The word you used to name your macro is a saved word. Try using a different name.\n");
        break;
        
    case FAILED_TO_ALLOCATE_MEMORY:
        fprintf(stderr, "The memory allocation has failed. Try correcting your malloc function.\n");
        break;

    case EXCEEDED_PERMISSIBLE_LENGTH:
        fprintf(stderr, "At least one of the lines in the input file is longer than 80 characters.\n");
        break;

    case DEFINE_SYNTAX_ERROR:
        fprintf(stderr, "You've got a syntax error in one of your define statements.\nMake sure your format is as follows: .define string = number\n");
        break;

    case NAME_IS_DUPLICATE:
        fprintf(stderr, "The name you've chosen for one of your tags or define statements is a duplicate.\n");
        break;

    case USED_SAVED_WORD_AS_TAG:
        fprintf(stderr, "The tag you've chosen is a saved word or a name of a macro. Try using a different name.\n");
        break;

    case USED_MACRO_AS_DEFINE:
        fprintf(stderr, "The name you've chosen for your define statement is a macro. Try using a different name.\n");
        break;

    default:
        fprintf(stderr, "An unknown error occurred.\n");
        break;
    }

    exit(1); // Terminate the program with a non-zero exit code
}