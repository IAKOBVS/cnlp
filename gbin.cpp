#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

#ifdef __cplusplus
}
#endif

#include <vector>
#include <string>

#include "/home/james/c/nix.c/nix.h"
#include "/home/james/c/macros/global_macros.h" // noc

#define NUM_ALPHA 52

#define PATH_TO_FILE "data/"
#define BIN_FILE "all_words.bin"

#define LETTER argv[1][0] 
#define WORD argv[1]

ALWAYS_INLINE CONST static int iter(const int i)
{
	switch (i) {
	CASE_UPPER
		return i - 39;
	CASE_LOWER
		return i - 79;
	}
	return 0;
}

ALWAYS_INLINE CONST static int iter_rev(const int i)
{
	switch (i) {
	CASE_UPPER
		return i - 65;
	CASE_LOWER
		return i - 71;
	}
	return 0;
}

#define dict_at_letter(l) d[dict_get_iter(l)].c_str()

[[nodiscard]] int dict_load(std::vector<std::string> *d)
{
	d->reserve(NUM_ALPHA);
	if (unlikely(d->capacity() != NUM_ALPHA))
		return 0;
	struct dirent *RESTRICT ep;
	DIR *RESTRICT dp = opendir(PATH_TO_FILE);
	char filename[] = "data/ ";
	filename[sizeof("data ")] = '\0';
	while ((ep = readdir(dp))) {
		char *buf;
		filename[sizeof("data")] = *(ep->d_name);
		size_t file_size = nix_cat_auto(&buf, filename);
		if (unlikely((!file_size)))
			return 0;
		(*d)[iter(*ep->d_name)].append(buf);
		free(buf);
	}
	closedir(dp);
	return 1;
}

[[nodiscard]] ALWAYS_INLINE CONST const char *dict_is_word(const std::vector<std::string> *d, const char* word)
{
	return (strstr((*d)[iter(*word)].c_str(), word));
}

[[nodiscard]] ALWAYS_INLINE CONST const char *dict_is_word_nocase(const std::vector<std::string> *d, const char* word)
{
	return strstr((*d)[iter(*word)].c_str(), word) ?: strstr((*d)[iter_rev(*word)].c_str(), word);
}

ALWAYS_INLINE void match_word_print(const char *word)
{
	for (;;) {
		switch (*word) {
		default:
			putchar(*word++);
			continue;
		case '\0':
		case '\n':;
		}
		break;
	}
}

int main(int argc, char **argv)
{
	if (unlikely(argc == 1)) {
		puts("Usage: ./gbin word");
		return -1;
	}
	std::vector<std::string> d;
	if (unlikely(!dict_load(&d))) {
		perror("alloc failed");
		return -1;
	}
	const char *ret = dict_is_word_nocase(&d, WORD);
	match_word_print(ret);
	return 0;
}
