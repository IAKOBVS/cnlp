#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <iostream>
#include <vector>
#include <string>
#include <assert.h>

#include "/home/james/c/nix.c/nix.h"
#include "/home/james/c/macros/global_macros.h" // noc

/* #define filename "data-raw/wiktionary-all-words.bin" */
#define PATH_TO_FILE "./data/"

int main()
{
	std::vector<std::string> dict;
	dict.reserve(51);
	struct dirent *RESTRICT ep;
	DIR *RESTRICT dp = opendir(PATH_TO_FILE);
	for (char *RESTRICT filename; (ep = readdir(dp)); ) {
		filename = ep->d_name;
		int i;
		switch (*filename) {
		CASE_UPPER
			i = *filename - 39;
			break;
		CASE_LOWER
			i = *filename - 97;
		}
		char *buf;
		nix_cat_auto(&buf, filename);
		free(buf);
	}
}
