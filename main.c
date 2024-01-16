#include <stdio.h>
#include <stdlib.h>
#include "errors.h"

int main(int argc, char* argv[]){
    if(argc < 2)
        error_output(1);
    
    return 1;
}