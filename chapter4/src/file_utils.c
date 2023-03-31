#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#include "file_utils.h"
#include "common.h"

/* return the suffix of a string of filename after the right most '.'.
 * if none return the whole string as suffix
 */
char *suffix(char *fname)
{
	if (fname == NULL) return NULL;
	char* dot_str = strrchr(fname, '.');
    if (dot_str == NULL) return fname;

	return dot_str+1;
}

bool is_txt(char *fname)
{
	const size_t suffix_len = strnlen(TXT_SUFFIX, MAX_WORD_LEN);
	const size_t fname_len = strnlen(fname, MAX_STR_LEN);

	if (fname == NULL || fname_len <= suffix_len)
		return false;

	const char *fname_suffix = suffix(fname);
	if (fname_suffix == NULL) return false;
	return (strncmp(fname_suffix, TXT_SUFFIX, fname_len) == 0);
}

/* Return a LEN_STR_RRAY of file paths of all files ending in .txt under dir_path,
 * up to max_files, return NULL if error or empty LEN_STR_ARRAY if no files found.
 *
 * Caller should free string memories in file_paths when done!
 */
LEN_STR_ARRAY *find_txt_files_in_dir(char *dir_path, int max_files)
{
	struct dirent *entry;
	DIR *dp = opendir(dir_path);
	if (dp == NULL)
	{
		fprintf(stderr, "opendir of %s failed, abort.\n", dir_path);
		exit(1);
	}

	LEN_STR_ARRAY *fpaths = init_len_str_array();
	if (fpaths == NULL)
	{
		fprintf(stderr, "Failed to init len_str_array for file paths, abort!\n");
		exit(1);
	}

	char *fpath = malloc(MAX_STR_LEN+1);
	if (fpath == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for file path of size %ld, abort!\n", MAX_STR_LEN);
		exit(1);
	}
	while ((entry = readdir(dp)))
	{
		char *fname = entry->d_name;
		if (entry->d_type == 8 /*DT_REG*/ && is_txt(fname) && fpaths->len < max_files)
		{
			sprintf(fpath, "%s/%s", dir_path, fname);
			fpaths = append_len_str_array(fpaths, fpath);
			if (fpaths == NULL)
			{
				fprintf(stderr, "Failed appending path %s into results, abort!\n", fpath);
				exit(1);
			}
		}
	}
	closedir(dp);
	free(fpath);

	return fpaths;
}

/* Function to process an ASCII only txt file,
 * return null if error.
 * Caller must call free_digest() to free memory of returned results when done!
 */
FILE_DIGEST *process_file(char *file_path, int max_words)
{
	FILE *f = fopen(file_path, "r");
	if (f == NULL)
	{
		fprintf(stderr, "Failed to open %s for reading.\n", file_path);
		exit(1);
	}

	size_t buf_size = MAX_LINE_LEN;
	char *buf = malloc(buf_size+1);
	if (buf == NULL)
	{
		fprintf(stderr, "Failed to allocate buffer of size %ld for reading file %s, abort!\n", buf_size, file_path);
		exit(1);
	}

	size_t digest_size = sizeof(FILE_DIGEST);
	FILE_DIGEST *fdigest = malloc(digest_size);
	if (fdigest == NULL)
	{
		fprintf(stderr, "Failed to allocate FILE_DIGEST of size %ld for reading, abort!\n", digest_size);
		exit(1);
	}

	fdigest->num_lines = 0;
	fdigest->total_size = 0;
	fdigest->unique_words = init_len_str_array();
	if (fdigest->unique_words == NULL)
	{
		fprintf(stderr, "Failed to init len_str_array for fdigest->unique_words, abort!\n");
		exit(1);
	}

	long read = 0;
	while ((read = getline(&buf, &buf_size, f)) > 0)
	{
		fdigest->num_lines++;
		fdigest->total_size += read;
		char *tok;
		// printf("Read Line: %s\n", buf);
		if (read > 0)
		{
			for (tok = strtok(buf, TOK_DELIM);
				 tok != NULL && fdigest->unique_words->len < MAX_NUM_WORDS;
				 tok = strtok(NULL, TOK_DELIM))
			{
				char *low_tok = dup_lower(tok);
				if (low_tok == NULL)
				{
					fprintf(stderr, "Failed to dup_lower() on token %s, abort!\n", tok);
					exit(1);
				}
				tok = low_tok;
				//printf("token: %s\n", tok);

				fdigest->unique_words = append_len_str_array_if_new(fdigest->unique_words, tok);
				if (fdigest->unique_words == NULL)
				{
					fprintf(stderr, "Failed to append %s into unique words, abort!\n", tok);
					exit(1);
				}
				else if (fdigest->unique_words->len >= MAX_NUM_WORDS)
				{
					fprintf(stderr, "Num of unique words %d exceeded limit of %d, skip token processing, abort!\n", fdigest->unique_words->len, MAX_NUM_WORDS);
					exit(1);
				}
			}
		}
	}
	fclose(f);
	free(buf);

	return fdigest;
}

/* Function to properly free memory used by FILE_DIGEST*/
void free_digest(FILE_DIGEST *d)
{
	free_len_str_array(d->unique_words);
	d->unique_words = NULL;
	free(d);
}