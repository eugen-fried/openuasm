#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#ifndef SPLIT
#define SPLIT
typedef struct Split {
    char *head;
    char *tail;
}Split;
#endif

bool is_empty_string(const char *);
bool starts_with_char(const char *, char);
bool ends_with_char(const char *, char);
char *trim_whitespace(char* );
Split *split_string(const char*, char);
size_t strlcpy(char*, const char*, size_t);