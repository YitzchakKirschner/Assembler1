/* Symbol Table Entry Structure */
typedef struct Symbol {
    char name[MAX_MACRO_NAME_LENGTH]; // Symbol name
    int value;                        // Value of the symbol
    enum { MDEFINE, DATA, CODE, EXTERN } type; // Type of the symbol
    struct Symbol *next;              // Pointer to the next symbol
} Symbol;

extern int IC;
extern int DC;

FILE* firstRun(FILE* as_file_ptr, char* file_name, Symbol **symbolTable, MacroNode* macros, int IC, int DC);
void processDefineStatement(char *line, Symbol **symbolTable);
void processDataDirective(char *line, Symbol **symbolTable, FILE *output_file_ptr, int *DC, int data_type, char* first_word, int tag_flag);
void processCodeDirectives(char *line, Symbol **symbolTable);
void insertIntoSymbolTable(Symbol **current_symbol, char *name, int value, int type);
void freeSymbolTable(Symbol *symbolTable);
int isTag(char *word, MacroNode* macros);
void writeBinaryNumbersToFile(FILE* file, const char* numbers, int* DC);
void writeAsciiBinaryToFile(FILE* file, const char* str, int* DC);

#define DATA_CODE 1
#define STRING_CODE 2
