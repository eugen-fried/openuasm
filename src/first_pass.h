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
#include "symbols.h"
#define MAX_LINE_SIZE 100
#define SYMBOLS_TABLE_SIZE 100
#define STARTING_POINT 0x144


#define NONE -1
#define INVALID -10

#ifndef FIRST_PASS
#define FIRST_PASS

enum Instr {
    data, string, entry, extrn
};

enum OpertType {
    MOV, CMP, ADD, SUB, NOT, CLR, LEA, INC, DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP
};

enum IndexType {
    REGISTER, IMMEDIATE, REFERENCE
};



typedef struct Operation {
    enum OpertType opertType;
    bool dbl, type;
    int cmb, dest_reg, dest_adr, source_reg, source_adr;
    int source_oprnd;
    int dest_oprnd;
    int source_indx;
    int dest_index;
} Operation;

extern int data_area[2000];
extern int opr_area[2000];
extern int dc, ic;
extern Symbol *symbol_table[HASH_TAB_SIZE];

#endif




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
int get_opert_type(char *);
bool valid_index_oper(char*);
int get_index_type(char *, int*);
char *get_index_expr(char*, int*);
int get_register_code(char *);
int get_binary_length(char *line);
int get_single_operand_info(char*, int*, int*);
void handle_error(int, char*);
int handle_operation(char*);
int handle_instr(char*);
int calc_code_length(char*);
bool get_line(char*);
bool is_binary_operation(enum OpertType);
