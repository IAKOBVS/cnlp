#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "/home/james/c/nix.c/nix.h"
#include "/home/james/c/macros/global_macros.h" // noc

#define filename "data-raw/wiktionary-all-words"
#define MIN_WORDS 320000
#define NUM_OF_ALPHA 51
#define MAX_WORD_LENGTH 1024

struct Dict {
	unsigned char c;
	char *data[];
} Dict;

int main()
{
	struct Dict *d = malloc(NUM_OF_ALPHA * sizeof(Dict) + NUM_OF_ALPHA * MIN_WORDS * sizeof(char *));
	char *file;
	size_t file_size = nix_cat_auto(&file, filename);
	if (!file_size)
		return -1;
	char *fp = file;
	for (;;) {
		char *datap;
		switch (*fp) {
		CASE_UPPER
			datap = *d[*fp - 39].data;
			break;
		CASE_LOWER
			datap = *d[*fp - 97].data;
			break;
		case '\0':
			goto OUT;
		default:
			fp = strchr(fp, '\n');
			if (!fp)
				goto ERROR_FREE;
			++fp;
			continue;
		}
		for (;;) {
			switch (*fp) {
			default:
				*datap++ = *fp++;
				continue;
			case '\n':
				*datap++ = ' ';
				*datap = '\0';
				break;
			case '\0':
				*datap = '\0';
				goto OUT;
			}
			break;
		}
	}
OUT:
	free(file);
	return 0;

ERROR_FREE:
	free(file);
ERROR:
	perror("");
	return -1;
}
