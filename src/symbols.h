/* 
 * File:   symbols.h
 * Author: evgeny
 *
 * Created on August 3, 2013, 5:32 PM
 */

#include <string.h>
#include "../lib/sglib/sglib.h"
#include <stdbool.h>

#ifndef SYMBOLS_H
#define	SYMBOLS_H
typedef struct Symbol {
    char *name;
    int address;
    bool is_extern;
    bool has_inst;
    struct Symbol *next;
} Symbol;

#define HASH_TAB_SIZE  512
#define SYMBOL_COMPARATOR(e1, e2) (e1 -> address - e2 -> address)

unsigned int symbol_hash_funct(Symbol*);

SGLIB_DEFINE_LIST_PROTOTYPES(Symbol, SYMBOL_COMPARATOR, next)

SGLIB_DEFINE_HASHED_CONTAINER_PROTOTYPES(Symbol, HASH_TAB_SIZE, symbol_hash_funct)
#endif

