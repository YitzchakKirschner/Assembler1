
#include "util.h"


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
//void writeDataToFile(OutputLines* data_output_head, FILE* output_file_ptr);
void processCodeDirective(char *line, Symbol **symbolTable, int *IC, int* src_line, char* first_word, int tag_flag, OutputLines** instruction_output, OutputLines** data_output);
void processInstruction(char* instruction, char* line, Symbol** symbolTable, int* IC, int* src_line, OutputLines** instruction_output, OutputLines** data_output);

#define DATA_CODE 1
#define STRING_CODE 2
