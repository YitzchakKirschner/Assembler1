#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "saved_words.h"
#include "errors.h"

Word* defineLanguage() {
    Word *head = NULL, *temp = NULL, *current = NULL;
    /* Create and link the Words*/
    temp = createWord("mov", 0, "0123", "123");
    head = temp;
    current = temp;
    CREATE_AND_LINK(temp, current, "cmp", 1, "0123", "0123");
    CREATE_AND_LINK(temp, current, "add", 2, "0123", "123");
    CREATE_AND_LINK(temp, current, "sub", 3, "0123", "123");
    CREATE_AND_LINK(temp, current, "not", 4, NULL, "123");
    CREATE_AND_LINK(temp, current, "clr", 5, NULL, "123");
    CREATE_AND_LINK(temp, current, "lea", 6, "12", "123");
    CREATE_AND_LINK(temp, current, "inc", 7, NULL, "123");
    CREATE_AND_LINK(temp, current, "dec", 8, NULL, "123");
    CREATE_AND_LINK(temp, current, "jmp", 9, NULL, "13");
    CREATE_AND_LINK(temp, current, "bne", 10, NULL, "13");
    CREATE_AND_LINK(temp, current, "red", 11, NULL, "123");
    CREATE_AND_LINK(temp, current, "prn", 12, NULL, "0123");
    CREATE_AND_LINK(temp, current, "jsr", 13, NULL, "13");
    CREATE_AND_LINK(temp, current, "rts", 14, NULL, NULL);
    CREATE_AND_LINK(temp, current, "hlt", 15, NULL, NULL);
        

    /* Display the list */
    current = head;
    while (current != NULL) {
        printf("Name: %s, Code: %d, Src: %s, Dst: %s\n",
               current->name, current->code, current->src_reg_type, current->dst_reg_type);
        current = current->next;
    }

    return head;
}

// Function to create a new Word
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
int isSavedWord(char first_field[], Word* head){
    while(head) {
        if(strcmp(head->name, first_field) == 0){
            return -1;
        }
        head = head->next;
    }
    return 0;
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