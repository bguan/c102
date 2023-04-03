#include <stdio.h>
#include "common.h"

int main(int argc, char **argv)
{
	STR_ARRAY *words = init_str_array();

	append_str_array(words, (char *)"Uno");
	append_str_array(words, (char *)"Dos");
	append_str_array(words, (char *)"Tres");

	for(int wi = 0; wi < words->len; wi++)
	{
		printf("%d: %s\n", wi, words->strs[wi]);
    }

	free_str_array(words);
	return 0;
}
