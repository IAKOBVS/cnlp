#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "/home/james/c/nix.c/nix.h"

#define filename "data-raw/wiktionary-all-words"

#define WORD argv[1]
#define MAX_WORD_LENGTH 1024

int main(int argc, char **argv)
{
	char *buf;
	if (unlikely(!nix_cat_auto(&buf, filename)))
		return -1;
	char *start = buf;
	char word[MAX_WORD_LENGTH];
	word[0] = '\n';
	size_t word_len = strlen(WORD);
	memcpy(word + 1, WORD, word_len);
	word[word_len + 1] = '\0';
	for (;;) {
		start = strstr(start, word);
		if (unlikely(!start)) {
			free(buf);
			return -1;
		}
		++start;
		while (*start != '\n')
			putchar(*start++);
		putchar('\n');
	}
	free(buf);
	return 0;
}
