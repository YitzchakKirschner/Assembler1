/* Symbol Table Entry Structure */
typedef struct Symbol {
    char name[81]; // Symbol name
    int value;                        // Value of the symbol
    enum { MDEFINE, DATA, CODE, EXTERN } type; // Type of the symbol
    struct Symbol *next;              // Pointer to the next symbol
} Symbol;

int countLinesInFile(FILE* file);
void toBinary(int number, int n, char *binary, int useTwosComplement);
char* removeBracketedNumber(char *str, int *number, Symbol** symbolTable);
Symbol* findSymbol(Symbol* symbolTable, char* name, int type);
char* getRegisterByName(char* name);