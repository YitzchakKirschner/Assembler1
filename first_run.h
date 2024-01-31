/* Symbol Table Entry Structure */
typedef struct Symbol {
    char name[MAX_MACRO_NAME_LENGTH]; // Symbol name
    int value;                        // Value of the symbol
    enum { MDEFINE, DATA, CODE, EXTERN } type; // Type of the symbol
    struct Symbol *next;              // Pointer to the next symbol
} Symbol;

extern int IC;
extern int DC;

FILE* firstRun(FILE* as_file_ptr, char* file_name);
void processDefineStatement(char *line, Symbol **symbolTable);
void processDataDirectives(char *line, Symbol **symbolTable);
void processCodeDirectives(char *line, Symbol **symbolTable);
int insertIntoSymbolTable(Symbol **symbolTable, char *name, int value, int type);
void processDefineStatement(char *line, Symbol **symbolTable);
void processDataDirectives(char *line, Symbol **symbolTable, int *DC);
void processCodeDirectives(char *line, Symbol **symbolTable, int *IC);
int insertIntoSymbolTable(Symbol **symbolTable, char *name, int value, int type);
void processDefineStatement(char *line, Symbol **symbolTable) ;
void processDataDirectives(char *line, Symbol **symbolTable, int *DC);
void processCodeDirectives(char *line, Symbol **symbolTable, int *IC);
void processDataDirectives(char *line, Symbol **symbolTable, int *DC);
void processCodeDirectives(char *line, Symbol **symbolTable, int *IC);
