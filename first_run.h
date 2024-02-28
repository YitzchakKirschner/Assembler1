/* Symbol Table Entry Structure */
typedef struct Symbol {
    char name[MAX_MACRO_NAME_LENGTH]; // Symbol name
    int value;                        // Value of the symbol
    enum { MDEFINE, DATA, CODE, EXTERN } type; // Type of the symbol
    struct Symbol *next;              // Pointer to the next symbol
} Symbol;

extern int IC;
extern int DC;

FILE* firstRun(FILE* as_file_ptr, char* file_name, Symbol *symbolTable);
void processDefineStatement(char *line, Symbol *symbolTable);
void processDataDirectives(char *line, Symbol *symbolTable);
void processCodeDirectives(char *line, Symbol *symbolTable);
int insertIntoSymbolTable(Symbol *current_symbol, char *name, int value, int type);
void freeSymbolTable(Symbol *symbolTable);