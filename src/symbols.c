/* 
 * File:   symbols.c
 * Author: evgeny
 *
 * Created on August 3, 2013, 5:32 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "symbols.h"

unsigned int symbol_hash_funct(Symbol *symbol) {
    char *str = symbol -> name;
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

SGLIB_DEFINE_LIST_FUNCTIONS(Symbol, SYMBOL_COMPARATOR, next)

SGLIB_DEFINE_HASHED_CONTAINER_FUNCTIONS(Symbol, HASH_TAB_SIZE, symbol_hash_funct)
    
/* Don't know why SGLib didn't do that this, simple hash way*/
Symbol *get_symbol_by_name(Symbol **symbol_table, char *name) {
    Symbol query, *result;
    query.name = name;
    result = (symbol_table[symbol_hash_funct(&query) % HASH_TAB_SIZE]);
    return result;
}