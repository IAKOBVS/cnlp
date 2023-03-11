#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include <assert.h>

#include "/home/james/c/nix.c/nix.h"
#include "/home/james/c/macros/global_macros.h" // noc

#define filename "data-raw/wiktionary-all-words.bin"

int main()
{
	std::vector<std::string> dict;
	dict.resize(51);
	char *file;
	if (!nix_cat_auto(&file, filename))
		return -1;
	char *fp = file;
	for (;;) {
		int i;
		switch (*fp) {
		CASE_UPPER
			i = *fp - 39;
			break;
		CASE_LOWER
			i = *fp - 97;
			break;
		case '\0':
			goto OUT;
		default:
			fp = strchr(fp, '\n');
			if (!fp)
				goto OUT;
			++fp;
			continue;
		}
		size_t len = 0;
		for (;;) {
			switch (fp[len]) {
			default:
				++len;
				continue;
			case '\n':
			case '\0':
				++len;
				break;
			}
			break;
		}
		dict[i].reserve(len);
		for (;;) {
			switch (*fp) {
			default:
				dict[i].push_back(*fp++);
				continue;
			case '\n':
				dict[i].push_back(*fp++);
				break;
			case '\0':
				goto OUT;
			}
			break;
		}
		for (auto s : dict)
			std::cout << s << '\n';
		/* assert(2==3); */
	}
OUT:
	/* for (auto s : dict) */
	/* 	std::cout << s << '\n'; */
	free(file);
	return 0;

ERROR_FREE:
	free(file);
ERROR:
	perror("");
	return -1;
}
