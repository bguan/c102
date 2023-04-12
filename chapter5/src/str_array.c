#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "str_array.h"

#define PTR_SZ sizeof(char **)

STR_ARRAY *init_str_array()
{
    size_t sa_size = sizeof(STR_ARRAY);
    STR_ARRAY *sa = (STR_ARRAY *)malloc(sa_size);
    if (sa == NULL)
    {
        fprintf(stderr, "Failed to allocate memory of size %ld for STR_ARRAY, abort!\n", sa_size);
        exit(1);
    }

    sa->len = 0;
    sa->strs = NULL;
    sa->allocated = 0;
    return sa;
}

/* append a string by copying it into end of array, return NULL if error */
STR_ARRAY *append_str_array(STR_ARRAY *sa, char *new_str)
{
    if (sa == NULL)
    {
        fprintf(stderr, "ignoring attempt to append %s to NULL STR_ARRAY, returning NULL!\n", new_str);
        return NULL;
    }

    if (sa->allocated == 0)
    {
        sa->strs = (char **)calloc(GROWTH_FACTOR, PTR_SZ);
        if (sa->strs == NULL)
        {
            fprintf(stderr, "Failed to allocate memory to append %s to STR_ARRAY, abort!\n", new_str);
            exit(1);
        }
        sa->allocated = GROWTH_FACTOR;
    }
    else if (sa->len >= sa->allocated)
    {
        sa->allocated = GROWTH_FACTOR * sa->allocated;
        sa->strs = (char **)realloc(sa->strs, sa->allocated * PTR_SZ);
        if (sa->strs == NULL)
        {
            fprintf(stderr, "Failed to reallocate memory to append %s to STR_ARRAY, abort!\n", new_str);
            exit(1);
        }
    }

    char *new_str_cpy = strndup(new_str, MAX_STR_LEN);
    if (new_str_cpy == NULL)
    {
        fprintf(stderr, "Failed to duplicate %s, abort!\n", new_str);
        exit(1);
    }

    sa->strs[sa->len] = new_str_cpy;
    sa->len++;

    return sa;
}

/* find a string in array, return -1 if not found or error */
int find_str_array(STR_ARRAY *sa, char *str)
{
    if (sa == NULL)
    {
        fprintf(stderr, "ignoring attempt to find %s in a NULL STR_ARRAY, returning -1!\n", str);
        return -1;
    }

    for (int i = 0; i < sa->len; i++)
    {
        if (strncmp(str, sa->strs[i], MAX_STR_LEN) == 0)
            return i;
    }
    return -1;
}

/* remove an entry from array, return NULL if error */
STR_ARRAY *remove_str_array(STR_ARRAY *sa, int pos)
{
    if (sa == NULL)
    {
        fprintf(stderr, "ignoring attempt to remove %d'th entry in a NULL STR_ARRAY, returning NULL!\n", pos);
        return NULL;
    }

    if (pos >= sa->len)
        return sa;

    if (sa->strs[pos]!=NULL)
    {
        free(sa->strs[pos]);
        sa->strs[pos] = NULL;
    }

    char **to = sa->strs + pos;
    char **from = sa->strs + (pos + 1);
    char **end = sa->strs + (sa->len);
    memmove(to, from, (end - from) * PTR_SZ);
    sa->len--;
    sa->strs[sa->len] = NULL;
    return sa;
}

/* insert an entry into array, pushing entries below one slot down, return NULL if error */
STR_ARRAY *insert_str_array(STR_ARRAY *sa, int pos, char *new_str)
{
    if (sa == NULL)
    {
        fprintf(stderr, "ignoring attempt to insert %s to %d'th slot in a NULL STR_ARRAY, returning NULL!\n", new_str, pos);
        return NULL;
    }

    while (pos >= sa->allocated)
    {
        sa->allocated = GROWTH_FACTOR * sa->allocated;
        sa->strs = (char **)realloc(sa->strs, sa->allocated * PTR_SZ);
        if (sa->strs == NULL)
        {
            fprintf(stderr, "Failed to reallocate memory to append %s to STR_ARRAY, abort!\n", new_str);
            exit(1);
        }
    }

    if (pos >= sa->len)
    {
        sa->len = pos+1;
    } 
    else
    {
        char **from = sa->strs + pos;
        char **to = sa->strs + (pos + 1);
        char **end = sa->strs + sa->len;
        memmove(to, from, (end - from)*PTR_SZ);
        sa->len++;
    }

    char *new_str_cpy = strndup(new_str, MAX_STR_LEN);
    if (new_str_cpy == NULL)
    {
        fprintf(stderr, "Failed to duplicate %s, abort!\n", new_str);
        exit(1);
    }

    sa->strs[pos] = new_str_cpy;
    return sa;
}

/* free a string array */
void free_str_array(STR_ARRAY *sa)
{
    if (sa == NULL)
    {
        fprintf(stderr, "ignoring attempt to free a NULL STR_ARRAY!\n");
        return;
    }

    for (int si = 0; si < sa->len; si++)
    {
        if (sa->strs[si] != NULL) 
        {
            // printf("Free sa->strs[%d]\n", si);
            free(sa->strs[si]);
            sa->strs[si] = NULL;
        }
    }

    sa->len = 0;

    if (sa->strs != NULL)
    {
        // printf("Free sa->strs\n");
        free(sa->strs);
        sa->strs = NULL;
    }
    sa->allocated = 0;

    // printf("Free sa\n");
    free(sa);
}
