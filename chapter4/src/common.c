#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "common.h"

/* copy & convert all chars in word to lowercase using ctype.h */
char *dup_lower(const char *word)
{
    char *new_word = (char *)malloc(strnlen(word, MAX_WORD_LEN));
    if (new_word == NULL) {
        fprintf(stderr, "Error allocating memory to copy string: %s, abort!\n", word);
        exit(1);
    }
    for (int i = 0; word[i]; i++)
    {
        new_word[i] = isascii(word[i]) ? tolower(word[i]) : '?';
    }
    return new_word;
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
    LEN_STR_ARRAY *lsa = (LEN_STR_ARRAY *)malloc(lsa_size);
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
        lsa->strs = (char **)malloc(ARRAY_GROWTH_FACTOR *sizeof(char *));
        if (lsa->strs == NULL)
        {
            fprintf(stderr, "Failed to allocate memory to append %s to LEN_STR_ARRAY, abort!\n", new_str);
            exit(1);
        }
        lsa->allocated = ARRAY_GROWTH_FACTOR;
    }
    else if (lsa->len >= lsa->allocated)
    {
        char *entry0_pre_realloc = lsa->strs[0];
        if (entry0_pre_realloc == NULL)
        {
            fprintf(stderr, "LEN_STR_ARRAY 1st entry is NULL before reallocation while inserting %s, abort!\n", new_str);
            exit(1);
        }

        lsa->allocated = ARRAY_GROWTH_FACTOR * lsa->allocated;
        lsa->strs = (char **)realloc(lsa->strs, lsa->allocated * sizeof(char *));
        if (lsa->strs == NULL)
        {
            fprintf(stderr, "Failed to reallocate memory to append %s to LEN_STR_ARRAY, abort!\n", new_str);
            exit(1);
        }

        char *entry0_post_realloc = lsa->strs[0];
        if (entry0_post_realloc ==  NULL)
        {
            fprintf(stderr, "LEN_STR_ARRAY 1st entry is NULL after reallocation while inserting %s, abort!\n", new_str);
            exit(1);
        }

        if (strncmp(entry0_pre_realloc, entry0_post_realloc, MAX_STR_LEN) != 0)
        {
            fprintf(stderr, "LEN_STR_ARRAY 1st entry was %s, now %s after reallocation while inserting %s, abort!\n",
                    entry0_pre_realloc, entry0_post_realloc, new_str);
            exit(1);
        }
    }

    char *new_str_cpy = (char *)malloc(strnlen(new_str, MAX_STR_LEN) + 1);
    if (new_str_cpy == NULL)
    {
        fprintf(stderr, "Failed to allocate memory to copy %s, abort!\n", new_str);
        exit(1);
    }

    strncpy(new_str_cpy, new_str, MAX_WORD_LEN);
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
        //free(lsa->strs[si]);
        lsa->strs[si] = NULL;
    }

    //free(lsa);
}