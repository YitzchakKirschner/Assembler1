/* Symbol Table Entry Structure */
typedef struct Symbol {
    char name[81]; // Symbol name
    int value;                        // Value of the symbol
    enum { MDEFINE, DATA, CODE, EXTERN } type; // Type of the symbol
    struct Symbol *next;              // Pointer to the next symbol
} Symbol;

typedef struct OutputText {
    char text[15];
    int output_line_number;
    struct OutputText *next;
} OutputText;

/* Simple Data Structure */
typedef struct OutputLines {
    OutputText *firstLine;  /* Pointer to the first line*/
    int is_decoded;            // Flag indicating if the data is used
    char src_operand[3];
    char dest_operand[3];
    char opcode[5];               // Opcode
    struct OutputLines* next;    // Pointer to the next data structure
} OutputLines;



int countLinesInFile(FILE* file);
void toBinary(int number, int n, char *binary, int useTwosComplement);
char* removeBracketedNumber(char *str, int *number, Symbol** symbolTable);
Symbol* findSymbol(Symbol* symbolTable, char* name, int type);
char* getRegisterByName(char* name);
void freeInstruction(OutputLines* instruction_output);
void addInstrctionLine(OutputLines* current, OutputText* newOutputText, int* IC, char* src_bin_word, char* dst_bin_word, char* src_ARE, char* dst_ARE, int srcExtractedNumber, int dstExtractedNumber);
void decodeInstruction(char* instruction, Symbol** symbolTable, OutputLines* current, OutputLines** data_output, char* word, char* ARE, int* extractedNumber, char** bin_word, char* tempLine, Symbol* found_symbol, char* token, int i, char* constArr);
int checkRegType(int ARE, const char* commandName, int isSrc);