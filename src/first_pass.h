/* 
 * File:   first_pass.h
 * Author: evgeny
 *
 * Created on July 4, 2013, 7:16 PM
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "io.h"
#include "mystring.h"
#include "../lib/queue.h"
#define MAX_LINE_SIZE 100
#define SYMBOLS_TABLE_SIZE 100
#define STARTING_POINT 0x144


#define NONE -1
#define INVALID -10
enum Instr {
    data, string, entry, extrn
};

enum Operation {
    MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP
};

enum IndexType {
    REGISTER, IMMEDIATE, REFERENCE 
};

typedef struct Symbol {
    char *name;
    int address;
    bool is_extern;
    bool has_inst;
    TAILQ_ENTRY(Symbol) pointers;
} Symbol;

typedef struct SymbolTable {
    TAILQ_HEAD(symbol_table, Symbol) head;
} SymbolTable;


extern int data_area[2000];
extern int opr_area[2000];
extern int dc, ic;
extern SymbolTable symbol_table;





bool is_meaningless_line(char*);
bool has_label(char*);
void notify_error(char*, int);
int get_instr(char*);
char *copy_line(char*);
char *get_label_name(char*);
char *get_symbol_name(char*);
char *remove_label(char *);
char *get_string_data(char*);
bool add_symbol(char*, int, bool, bool);
void handle_string_instr(char*);
void handle_data_instr(char*);
void first_pass();
int get_operation(char *);
bool valid_index_oper(char*);
int get_index_type(char *);
char *get_index_expr(char*, int*);
int get_register_code(char *);
int get_binary_length(char *line);
int get_single_operand_length(char *oper);




