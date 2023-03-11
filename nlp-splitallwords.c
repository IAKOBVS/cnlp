#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "/home/james/c/nix.c/nix.h"
#include "/home/james/c/macros/global_macros.h" // noc

#define filename "data-raw/wiktionary-all-words"
#define PATH "data/"
/* #define PATH "/tmp" */
#define MIN_WORDS 320000
#define NUM_OF_ALPHA 51
#define MAX_WORD_LENGTH 1024

int main()
{
	char *file;
	size_t file_size = nix_cat_auto(&file, filename);
	if (!file_size)
		return -1;
	char *filep = file;
	char path[strlen(PATH) + 2];
	strcpy(path, PATH);
	path[strlen(PATH) + 1] = '\0';
	FILE *fp;
	for (;;) {
		switch (*filep) {
		CASE_UPPER
		CASE_LOWER
			break;
		case '\0':
			goto OUT;
		default:
			filep = strchr(filep, '\n');
			if (!filep)
				goto ERROR_FREE;
			++filep;
			continue;
		}
		path[strlen("data/")] = *filep;
		fp = fopen(path, "a");
		if (!fp)
			goto ERROR_FREE;
		for (;;) {
			switch (*filep) {
			default:
				fputc(*filep++, fp);
				continue;
			case '\n':
				fputc(*filep++, fp);
				break;
			case '\0':
				fclose(fp);
				goto OUT;
			}
			break;
		}
		fclose(fp);
	}
OUT:
	free(file);
	return 0;

ERROR_FREE:
	free(file);
ERROR_CLOSE:
	fclose(fp);
ERROR:
	perror("");
	return -1;
}
