#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <strings.h>
#include <dirent.h>

#include "common.h"
#include "file_utils.h"

#define MAX_FILES 100

LEN_STR_ARRAY *unique_words;
long num_lines = 0L;
size_t total_size = 0L;

void combine(FILE_DIGEST *digest)
{
	num_lines += digest->num_lines;
	total_size += digest->total_size;

	if (unique_words->len >= MAX_NUM_WORDS)
		return;

	for (int fi = 0; fi < digest->unique_words->len; fi++)
	{
		char *uword = digest->unique_words->strs[fi];
		unique_words = append_len_str_array_if_new(unique_words, uword);
	}
}

/* for calling stdlib qsort */
int cmpstr(const void *p1, const void *p2)
{
	/* The actual arguments to this function are "pointers to
	   pointers to char", but strcmp(3) arguments are "pointers
	   to char", hence the following cast plus dereference. */
	return strcmp(*(const char **)p1, *(const char **)p2);
}

int main(int argc, char **argv)
{
	if (argc <= 1)
	{
		fprintf(stderr, "Usage: process path_to_directory\n");
		exit(1);
	}

	char *dir_name = argv[1];
	LEN_STR_ARRAY *txt_fpaths = find_txt_files_in_dir(dir_name, MAX_FILES);
	if (txt_fpaths == NULL)
	{
		fprintf(stderr, "Error processing directory: %s, abort!\n", dir_name);
		exit(1);
	}

	unique_words = init_len_str_array();
	for (int i = 0; i < txt_fpaths->len; i++)
	{
		char *fpath = txt_fpaths->strs[i];
		if (fpath == NULL || strlen(fpath) <= 0)
		{
			fprintf(stderr, "File path is NULL or empty, abort!\n");
			exit(1);
		}
		printf("Processing: %s\n", fpath);

		FILE_DIGEST *digest = process_file(fpath, MAX_NUM_WORDS);
		if (digest == NULL)
		{
			fprintf(stderr, "Error processing file: %s, abort!\n", fpath);
			exit(1);
		}
		else
		{
			combine(digest);
			free_digest(digest);
		}
	}

	printf("Total Files: %d\n", txt_fpaths->len);
	printf("Average lines in files: %ld\n", num_lines / txt_fpaths->len);
	printf("Average size of files: %ld\n", total_size / txt_fpaths->len);
	printf("Average line size in files: %ld\n", total_size / num_lines / txt_fpaths->len);
	printf("Number of Unique Words: %d\n", unique_words->len);
	printf("Unique Words:\n");
	for (int i = 0; i < unique_words->len; i++)
	{
		printf(i < unique_words->len - 1 ? "%s, " : "%s\n", unique_words->strs[i]);
	}

	qsort(unique_words->strs, unique_words->len, sizeof(char *), cmpstr);
	printf("\nUnique Words (sorted):");
	for (int i = 0; i < unique_words->len; i++)
	{
		printf((i <= 0 || unique_words->strs[i][0] != unique_words->strs[i - 1][0]) ? "\n\n%s" : ", %s", unique_words->strs[i]);
	}
	printf("\n");

	free_len_str_array(txt_fpaths);
	txt_fpaths = NULL;
	free_len_str_array(unique_words);
	unique_words = NULL;

	return 0;
}
