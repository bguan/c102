#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>

#define MAX_WORD_LEN 100L
#define MAX_STR_LEN 1000L

/* return if new_str is found in strings, up to max entries */
bool is_in(char **strings, char *new_str, int max_entries);

/* copy & convert all chars in word to lowercase using ctype.h */
char *dup_lower(const char *word);

// used to grow the LEN_STR_ARRAY to avoid calling realloc too many times
#define ARRAY_GROWTH_FACTOR 5

/* a standard way of pairing length with a string array */
typedef struct {
    unsigned int len;
    unsigned int allocated;
    char **strs;
} LEN_STR_ARRAY;

/* create an empty string array */
LEN_STR_ARRAY *init_len_str_array();

/* append a string by copying it into end of array, return NULL if error */
LEN_STR_ARRAY *append_len_str_array(LEN_STR_ARRAY *lsa, char *new_str);

/* if new_str is not already in array, append by copying it, return NULL if error */
LEN_STR_ARRAY *append_len_str_array_if_new(LEN_STR_ARRAY *lsa, char *new_str);

/* free a string array */
void free_len_str_array(LEN_STR_ARRAY *lsarr);

#endif
