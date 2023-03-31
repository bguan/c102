#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "common.h"

/* copy & convert all chars in word to lowercase using ctype.h */
char *dup_lower(const char *word)
{
    char *w = strndup(word, MAX_STR_LEN);
    if (w == NULL) {
        fprintf(stderr, "Error duplicating string: %s, abort!\n", word);
        exit(1);
    }
    for (int i = 0; w[i]; i++)
    {
        w[i] = isascii(w[i]) ? tolower(w[i]) : '?';
    }
    return w;
}

/* return if new_str is found in strings, up to max entries */
bool is_in(char **strings, char *new_str, int max_entries)
{
    if (strings == NULL && max_entries > 0) 
    {
        fprintf(stderr, "check is_in of %s but list is NULL, abort!\n", new_str);
        exit(1);
    }

    bool found = false;
    for (int si = 0; si < max_entries && !found; si++)
    {
        char *s = strings[si];
        found = (strncmp(s, new_str, MAX_WORD_LEN) == 0);
    }

    return found;
}

/* create an empty len_str_array */
LEN_STR_ARRAY *init_len_str_array()
{
    size_t lsa_size = sizeof(LEN_STR_ARRAY);
    LEN_STR_ARRAY *lsa = malloc(lsa_size);
    if (lsa == NULL)
    {
        fprintf(stderr, "Failed to allocate memory of size %ld for LEN_STR_ARRAY, abort!\n", lsa_size);
        exit(1);
    }

    lsa->len = 0;
    lsa->strs = NULL;
    lsa->allocated = 0;
    return lsa;
}

/* append a string by copying it into end of array, return NULL if error */
LEN_STR_ARRAY *append_len_str_array(LEN_STR_ARRAY *lsa, char *new_str)
{
    if (lsa->allocated == 0)
    {
        lsa->strs = malloc(ARRAY_GROWTH_FACTOR *sizeof(char *));
        if (lsa->strs == NULL)
        {
            fprintf(stderr, "Failed to allocate memory to append %s to LEN_STR_ARRAY, abort!\n", new_str);
            exit(1);
        }
        lsa->allocated = ARRAY_GROWTH_FACTOR;
    }
    else if (lsa->len >= lsa->allocated)
    {
        lsa->allocated = ARRAY_GROWTH_FACTOR * lsa->allocated;
        lsa->strs = (char **)realloc(lsa->strs, lsa->allocated * sizeof(char *));
        if (lsa->strs == NULL)
        {
            fprintf(stderr, "Failed to reallocate memory to append %s to LEN_STR_ARRAY, abort!\n", new_str);
            exit(1);
        }
    }

    char *new_str_cpy = strndup(new_str, MAX_STR_LEN);
    if (new_str_cpy == NULL)
    {
        fprintf(stderr, "Failed to duplicate %s, abort!\n", new_str);
        exit(1);
    }

    lsa->strs[lsa->len] = new_str_cpy;
    lsa->len++;

    return lsa;
}

/* if new_str is not already in array, append by copying it, return NULL if error */
LEN_STR_ARRAY *append_len_str_array_if_new(LEN_STR_ARRAY *lsa, char *new_str)
{
    if (lsa == NULL) 
    {
        fprintf(stderr, "Attempt to call append_len_str_array_if_new with NULL array, abort!\n");
        exit(1);
    }

    if (is_in(lsa->strs, new_str, lsa->len))
        return lsa;

    return append_len_str_array(lsa, new_str);
}

/* free a string array */
void free_len_str_array(LEN_STR_ARRAY *lsa)
{
    if (lsa == NULL) 
    {
        fprintf(stderr, "ignoring attempt to free a NULL LEN_STR_ARRAY, abort!\n");
        exit(1);
    }

    for (int si = 0; si < lsa->len; si++)
    {
        free(lsa->strs[si]);
        lsa->strs[si] = NULL;
    }

    free(lsa);
}