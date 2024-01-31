#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "saved_words.h"
#include "errors.h"

Word *head = NULL;

Word* defineLanguage() {
    Word *temp = NULL, *current = NULL;
    /* Create and link the Words*/
    temp = createWord("mov", 0, "0123", "123");
    head = temp;
    current = temp;
    CREATE_AND_LINK("cmp", 1, "0123", "0123");
    CREATE_AND_LINK("add", 2, "0123", "123");
    CREATE_AND_LINK("sub", 3, "0123", "123");
    CREATE_AND_LINK("not", 4, NULL, "123");
    CREATE_AND_LINK("clr", 5, NULL, "123");
    CREATE_AND_LINK("lea", 6, "12", "123");
    CREATE_AND_LINK("inc", 7, NULL, "123");
    CREATE_AND_LINK("dec", 8, NULL, "123");
    CREATE_AND_LINK("jmp", 9, NULL, "13");
    CREATE_AND_LINK("bne", 10, NULL, "13");
    CREATE_AND_LINK("red", 11, NULL, "123");
    CREATE_AND_LINK("prn", 12, NULL, "0123");
    CREATE_AND_LINK("jsr", 13, NULL, "13");
    CREATE_AND_LINK("rts", 14, NULL, NULL);
    CREATE_AND_LINK("hlt", 15, NULL, NULL);

    return head;
}

/*Function to create a new Word*/
Word* createWord(char name[], int code, char src_reg_type[], char dst_reg_type[]) {
    Word* newWord = (Word*)malloc(sizeof(Word));
    if(newWord){
        strcpy(newWord->name, name);
        newWord->code = code;
        if(src_reg_type)
            strcpy(newWord->src_reg_type, src_reg_type); 
        else
            newWord->src_reg_type[0] = '\0';

        if(dst_reg_type)
            strcpy(newWord->dst_reg_type, dst_reg_type); 
        else
            newWord->dst_reg_type[0] = '\0';

        newWord->next = NULL;
        return newWord;
    }
    error_output(4);/* failed to allocate memory */
    return NULL;
}

/* The function checks if first_field is a saved word */
int isSavedWord(char first_field[]){
    Word* temp = head;
    while(temp) {
        if(strcmp(temp->name, first_field) == 0){
            return 1;
        }
        temp = temp->next;
    }

    if(COMPARE_STRINGS(".data") || COMPARE_STRINGS(".entry") || COMPARE_STRINGS(".extern") || COMPARE_STRINGS(".string"))
        return 1;

    return 0; /* Not a saved word*/
}

/* Function to free language words */
void freeLanguage(Word* current){
    Word* temp = NULL;
    while (current) {
        temp = current;
        current = current->next;
        free(temp);
    }
}