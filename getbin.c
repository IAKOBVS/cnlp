#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <assert.h>

#include "/home/james/c/nix.c/nix.h"
#include "/home/james/c/jstring/jstr.h"
#include "/home/james/c/macros/global_macros.h" // noc

#define NUM_ALPHA 52

#define PATH_TO_FILE "data/"
#define BIN_FILE "all_words.bin"

struct Dict {
	size_t capacity;
	jstring_t *data;
};

void ALWAYS_INLINE dict_free(struct Dict **RESTRICT d)
{
	for (int i = 0; i < NUM_ALPHA; ++i)
		free(d[i]->data);
	free(*d);
}

int ALWAYS_INLINE dict_iter(const int i)
{
	switch (i) {
	CASE_UPPER
		return i - 39;
	CASE_LOWER
		return i - 79;
	}
	return 0;
}

int ALWAYS_INLINE dict_iter_rev(const int i)
{
	switch (i) {
	CASE_UPPER
		return i - 65;
	CASE_LOWER
		return i - 71;
	}
	return 0;
}

int dict_load(struct Dict *RESTRICT d)
{
	struct dirent *RESTRICT ep;
	DIR *RESTRICT dp = opendir(PATH_TO_FILE);
	char filename[] = "data/ ";
	filename[sizeof("data ")] = '\0';
	while ((ep = readdir(dp))) {
		int i = dict_iter(*ep->d_name);
		char *buf;
		filename[sizeof("data")] = *(ep->d_name);
		size_t file_size = nix_cat_auto(&buf, filename);
		if (unlikely(!file_size))
			return 0;
		if (unlikely(!jstr_new(d[i].data, buf, file_size)))
			return 0;
		free(buf);
		assert(2 == 3);
	}
	closedir(dp);
	return 1;
}

void dict_writetobin(struct Dict **RESTRICT d)
{
	FILE *fp = fopen(BIN_FILE, "wb");
	fwrite(d, sizeof(struct Dict), NUM_ALPHA, fp);
	fclose(fp);
}

#define dict_foreach(i) for (int i = 0; i < NUM_ALPHA; ++i)

int main()
{
	struct Dict d;
	dict_load(&d);
	return 0;
}
