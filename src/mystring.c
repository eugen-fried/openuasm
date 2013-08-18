#include <stdlib.h>

#include "mystring.h"

bool is_empty_string(const char *str){
    char curr_char = *str;
    int i;
    
    for(i = 0; curr_char != '\0'; i++) {
        if(!isspace(curr_char)) {
            return false;
        }
        curr_char = str[i];
    }
    
    return true;
}

bool starts_with_char(const char *str, char a) {
    if(str[0] == a) {
        return true;
    }
    return false;
}

bool ends_with_char(const char *str, char a) {
    if (str[(strlen(str) - 1)] == a) {
        return true;
    }
    return false;
}

char *trim_whitespace(char *str) {
    char *end;

    /* Trim leading space */
    while (isspace(*str)) str++;

    if (*str == 0) /* All spaces?*/
        return str;

    /* Trim trailing space */
    end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) end--;

    /* Write new null terminator */
    end++;
    *end = '\0';

    return str;
}

/* My own interpritation of strtok. Does not affects the original string.
 * Returns struct that consists of two parts of string, before the delimiter 
 * and after. */
Split *split_string(const char* str, char delim) {
    Split *result;
    int size_head=0;
    char *head, *tail;
    /* We need this guy to strcpy not from beginning */
    const char *tailp;
    
    for(;str[size_head] != delim; size_head++);
    
    if(str[size_head - 1] == '\0') {
        /* Didn't find anything */
        return NULL;
    }
    
    result = (Split*) malloc(sizeof (Split));
    /* Allocate space for head, don't forget + 1 for ending 0 character.*/
    head = (char *) malloc((size_head +1) * sizeof(char));
    strlcpy(head, str, size_head + 1);
    
    tail = (char *) malloc((strlen(str) - size_head) * sizeof(char));
    /* +1 to get rid of the space character*/
    tailp = (str + size_head +1);
    strcpy(tail, tailp);
    
    result -> head = head;
    result -> tail = tail;
    return result;
}

/* Taken from BSD sources. Because strncpy (must die) is not safe null term.  */
size_t strlcpy(char *dst, const char *src, size_t siz) {
    char *d = dst;
    const char *s = src;
    size_t n = siz;

    /* Copy as many bytes as will fit */
    if (n != 0) {
        while (--n != 0) {
            if ((*d++ = *s++) == '\0')
                break;
        }
    }

    /* Not enough room in dst, add NUL and traverse rest of src */
    if (n == 0) {
        if (siz != 0)
            *d = '\0'; /* NUL-terminate dst */
        while (*s++)
            ;
    }

    return (s - src - 1); /* count does not include NUL */
}

/* Not memory safe removal of string part until first space char. 
 * E.g make sandwich, ' ' -> sandwich */
char *remove_before_space(char *line) {
    char* copy = line;
    while(*copy != '\0') {
        if(isspace(*copy)) {
            return copy + 1;
        }
        copy++;
    }
    return line;
}

remove_inner_spaces(char *str) {
    
}