/* Define the word struct*/
typedef struct Word {
    char name[4];          
    int code;
    char src_reg_type[5];     
    char dst_reg_type[5];     
    struct Word* next;
} Word;

/* Macro to compare first_field of a macro with any other given string*/
#define COMPARE_STRINGS(input_string) (strcmp((first_field), (input_string)) == 0)

/* Macro to create a saved word*/
#define CREATE_AND_LINK(temp, current, name, word, src_reg_type, dst_reg_type) \
    do { \
        temp = createWord(name, word, src_reg_type, dst_reg_type); \
        current->next = temp; \
        current = current->next; \
    } while(0)

/* Prototype functions */
Word* createWord(char name[], int code, char src_reg_type[], char dst_reg_type[]);
Word* defineLanguage();
void freeLanguage(Word* head);
int isSavedWord(char first_field[], Word* head);
