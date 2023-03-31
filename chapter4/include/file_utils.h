#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "common.h"

/* Return a LEN_STR_RRAY of file paths of all files ending in .txt under dir_path,
 * up to max_files, return NULL if error or empty LEN_STR_ARRAY if no files found.
 *
 * Caller should free string memories in file_paths when done!
 */
LEN_STR_ARRAY* find_txt_files_in_dir(char *dir_path, int max_files);

#define MAX_NUM_WORDS 1000
#define MAX_LINE_LEN 1000
#define TXT_SUFFIX ".txt"
#define TOK_DELIM "0123456789`~@#$%^&*()-_=+[{]}\\|;:'\",<.>/?] \t\n\r"

/* Data structure holding processing results of each file */
typedef struct {
  LEN_STR_ARRAY* unique_words; // lowercase unique ASCII only words in file
  size_t total_size;
  int num_lines;
} FILE_DIGEST;

/* Function to process an ASCII only txt file,
 * return null if error.
 * Caller must call free_digest() to free memory of returned results when done!
 */
FILE_DIGEST *process_file(char *file_path, int max_words);

/* Function to properly free memory used by FILE_DIGEST*/
void free_digest(FILE_DIGEST *d);

#endif
