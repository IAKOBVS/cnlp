#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <assert.h>

#include "/home/james/c/nix.c/nix.h"
#include "/home/james/c/macros/global_macros.h" // noc

#define NUM_ALPHA 52

#define PATH_TO_FILE "data/"
#define BIN_FILE "all_words.bin"

struct Dict {
	size_t capacity;
	char *data;
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

int dict_alloc(struct Dict **RESTRICT d)
{
	*d = malloc(NUM_ALPHA * sizeof(struct Dict));
	if (!*d)
		return 0;
	struct dirent *RESTRICT ep;
	DIR *RESTRICT dp = opendir(PATH_TO_FILE);
	char filename[] = "data/ ";
	filename[sizeof("data ")] = '\0';
	while ((ep = readdir(dp))) {
		int i = dict_iter(*ep->d_name);
		char *buf;
		filename[sizeof("data")] = *(ep->d_name);
		size_t file_size = nix_cat_auto(&buf, filename);
		if (!file_size)
			return 0;
		d[i]->data = malloc(file_size + 1);
		if (!d[i]->data)
			return 0;
		d[i]->capacity = file_size;
		memcpy(d[i]->data, buf, file_size + 1);
		free(buf);
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
	struct Dict *d;
	assert(dict_alloc(&d));
	/* printf("%s\n", d[dict_iter('a')].data); */
	dict_free(&d);
	return 0;
}
