/* Symbol Table Entry Structure */
typedef struct Symbol {
    char name[MAX_MACRO_NAME_LENGTH]; // Symbol name
    int value;                        // Value of the symbol
    enum { MDEFINE, DATA, CODE, EXTERN } type; // Type of the symbol
    struct Symbol *next;              // Pointer to the next symbol
} Symbol;

/* Simple Data Structure */
typedef struct OutputLines {
    char data[16];          // Char array of 15 chars plus null terminator
    int line_number;        // Line number
    int is_used;            // Flag indicating if the data is used
    struct OutputLines* next;    // Pointer to the next data structure
} OutputLines;

typedef struct DecodedLines{
    int src_line_number;
    int output_line_number;
    int is_decoded;
    struct DecodedLines* next;
} DecodedLines;



FILE* firstRun(FILE* as_file_ptr, char* file_name, Symbol **symbolTable, MacroNode* macros, int IC, int DC);
void processDefineStatement(char *line, Symbol **symbolTable);
void processDataDirective(char *line, Symbol **symbolTable, int *DC, int* src_line, int data_type, char* first_word, int tag_flag, char* data_segment);
void processCodeDirectives(char *line, Symbol **symbolTable);
void insertIntoSymbolTable(Symbol **current_symbol, char *name, int value, int type);
void freeSymbolTable(Symbol *symbolTable);
int isTag(char *word, MacroNode* macros);
void writeBinaryNumbersToDataSegment(char* data_segment, const char* numbers, int* DC);
void writeAsciiBinaryToDataSegment(char* data_segment, const char* str, int* DC);
void freeOutputLines(OutputLines *output_lines);
void freeDecodedLines(DecodedLines *decodedLines);
void addLineToDecodedLines(DecodedLines** current_decoded_line, int src_line, int output_line, int is_decoded);
#define DATA_CODE 1
#define STRING_CODE 2
