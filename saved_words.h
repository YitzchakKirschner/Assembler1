/* Define the struct*/
typedef struct Word {
    char name[4];          
    int code;
    char src_reg_type[5];     
    char dst_reg_type[5];     
    struct Word* next;
} Word;

#define CREATE_AND_LINK(temp, current, name, word, src_reg_type, dst_reg_type) \
    do { \
        temp = createWord(name, word, src_reg_type, dst_reg_type); \
        current->next = temp; \
        current = current->next; \
    } while(0)

Word* createWord(char name[], int code, char src_reg_type[], char dst_reg_type[]);
Word* defineLanguage();
void freeLanguage(Word* head);
int isSavedWord(char first_field[], Word* head);
