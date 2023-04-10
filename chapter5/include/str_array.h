#ifndef STR_ARRAY_H
#define STR_ARRAY_H

#define GROWTH_FACTOR 4
#define MAX_STR_LEN 1000

typedef struct {
    int len;
    int allocated;
    char **strs;
} STR_ARRAY;

/* create an empty string array */
STR_ARRAY *init_str_array();

/* append a string by copying it into end of array, return NULL if error */
STR_ARRAY *append_str_array(STR_ARRAY *sa, char *new_str);

/* find a string in array, return -1 if not found or error */
int find_str_array(STR_ARRAY *sa, char *str);

/* remove an entry from array, return NULL if error */
STR_ARRAY *remove_str_array(STR_ARRAY *sa, int pos);

/* insert an entry into array, pushing entries below one slot down, return NULL if error */
STR_ARRAY *insert_str_array(STR_ARRAY *sa, int pos, char *new_str);

/* free memory used by a string array */
void free_str_array(STR_ARRAY *sa);

#endif
