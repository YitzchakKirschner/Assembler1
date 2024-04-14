// /* Symbol Table Entry Structure */
// typedef struct Symbol {
//     char name[MAX_MACRO_NAME_LENGTH]; // Symbol name
//     int value;                        // Value of the symbol
//     enum { MDEFINE, DATA, CODE, EXTERN } type; // Type of the symbol
//     struct Symbol *next;              // Pointer to the next symbol
// } Symbol;
#include "util.h"

typedef struct OutputText {
    char text[MAX_LINE_LENGTH_PLUS_1];
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




FILE* firstRun(FILE* as_file_ptr, char* file_name, Symbol **symbolTable, MacroNode* macros, int IC, int DC);
void processDefineStatement(char *line, Symbol **symbolTable, MacroNode* macro_head);
//Symbol* findSymbol(Symbol* symbolTable, char* name, int type);
void processDataDirective(char *line, Symbol **symbolTable, int *DC, int* src_line, int data_type, char* first_word, int tag_flag, OutputLines** data_output);
void insertIntoSymbolTable(Symbol **current_symbol, char *name, int value, int type);
void freeSymbolTable(Symbol *symbolTable);
int isTag(char *word, MacroNode* macros);
void writeBinaryNumbersToDataSegment(OutputLines** data_output, const char* numbers, int* DC);
void writeAsciiBinaryToDataSegment(OutputLines** data_output, const char* str, int* DC);
void freeOutputLines(OutputLines *output_lines);
void writeDataToFile(OutputLines* data_output_head, FILE* output_file_ptr);
void processCodeDirective(char *line, Symbol **symbolTable, int *IC, int* src_line, char* first_word, int tag_flag, OutputLines** instruction_output, OutputLines** data_output);
void processMov(char* line, Symbol** symbolTable, int* IC, int* src_line, OutputLines** instruction_output, OutputLines** data_output);

void processCmp(char* line, Symbol** symbolTable, int* IC, int* src_line, OutputLines** instruction_output, OutputLines** data_output);

void processAdd(char* line, Symbol** symbolTable, int* IC, int* src_line, OutputLines** instruction_output, OutputLines** data_output);

void processSub(char* line, Symbol** symbolTable, int* IC, int* src_line, OutputLines** instruction_output, OutputLines** data_output);

void processNot(char* line, Symbol** symbolTable, int* IC, int* src_line, OutputLines** instruction_output, OutputLines** data_output);

void processClr(char* line, Symbol** symbolTable, int* IC, int* src_line, OutputLines** instruction_output, OutputLines** data_output);

void processLea(char* line, Symbol** symbolTable, int* IC, int* src_line, OutputLines** instruction_output, OutputLines** data_output);

void processInc(char* line, Symbol** symbolTable, int* IC, int* src_line, OutputLines** instruction_output, OutputLines** data_output);

void processDec(char* line, Symbol** symbolTable, int* IC, int* src_line, OutputLines** instruction_output, OutputLines** data_output);

void processJmp(char* line, Symbol** symbolTable, int* IC, int* src_line, OutputLines** instruction_output, OutputLines** data_output);

void processBne(char* line, Symbol** symbolTable, int* IC, int* src_line, OutputLines** instruction_output, OutputLines** data_output);

void processRed(char* line, Symbol** symbolTable, int* IC, int* src_line, OutputLines** instruction_output, OutputLines** data_output);

void processPrn(char* line, Symbol** symbolTable, int* IC, int* src_line, OutputLines** instruction_output, OutputLines** data_output);

void processJsr(char* line, Symbol** symbolTable, int* IC, int* src_line, OutputLines** instruction_output, OutputLines** data_output);

void processRts(char* line, Symbol** symbolTable, int* IC, int* src_line, OutputLines** instruction_output, OutputLines** data_output);

void processHlt(char* line, Symbol** symbolTable, int* IC, int* src_line, OutputLines** instruction_output, OutputLines** data_output);
#define DATA_CODE 1
#define STRING_CODE 2
