#define CASE_VOWEL_LOWER case 'a': case 'i': case 'u': case 'e': case 'o':
#define CASE_VOWEL_UPPER case 'A': case 'I': case 'U': case 'E': case 'O':
#define CASE_VOWEL CASE_VOWEL_UPPER: CASE_VOWEL_LOWER

#define CASE_CONSONANT_LOWER case 'b': case 'c': case 'd': case 'f': case 'g': case 'h': case 'j': case 'k': case 'l': case 'm': case 'n': case 'p': case 'q': case 'r': case 's': case 't': case 'v': case 'w': case 'x': case 'y': case 'z':
#define CASE_CONSONANT_UPPER case 'B': case 'C': case 'D': case 'F': case 'G': case 'H': case 'J': case 'K': case 'L': case 'M': case 'N': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'V': case 'W': case 'X': case 'Y': case 'Z':
#define CASE_CONSONANT CASE_CONSONANT_UPPER: CASE_CONSONANT_LOWER

#define CASE_DIGIT case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
#define CASE_LOWER case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
#define CASE_UPPER case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':

#define CASE_ALPHA CASE_LOWER CASE_UPPER
#define CASE_ALPHANUM CASE_DIGIT CASE_ALPHA

#define CASE_WHITESPACE case '\n': case '\t': case '\r': case ' ':

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
# define RESTRICT restrict
#elif defined(__GNUC__) || defined(__clang__)
# define RESTRICT __restrict__
#elif defined(_MSC_VER)
# define RESTRICT __restrict
#else
# define RESTRICT
#endif

#if defined(__GNUC__) || defined(__clang__)
    #define ALWAYS_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
    #define ALWAYS_INLINE __forceinline inline
#else
    #define ALWAYS_INLINE inline
#endif

#if defined(__PRETTY_FUNCTION__)
	#define CURR_FUNC __PRETTY_FUNCTION__
#elif defined(__FUNCTION__)
	#define CURR_FUNC __FUNCTION__
#elif defined(__func__)
	#define CURR_FUNC __func__
#else
	#define CURR_FUNC ""
#endif

#if (defined(__GNUC__) && (__GNUC__ >= 3)) || (defined(__clang__) && __has_builtin(__builtin_expect))
  #define likely(x) __builtin_expect(!!(x), 1)
  #define unlikely(x) __builtin_expect(!!(x), 0)
#else
  #define likely(x) (x)
  #define unlikely(x) (x)
#endif

#define CONST

extern "C" {
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <assert.h>
}
#include <iostream>
#include <vector>
#include <string>

#include "/home/james/c/nix.c/nix.h"
/* #include "/home/james/c/macros/global_macros.h" // noc */

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

[[nodiscard]] ALWAYS_INLINE CONST const char *dict_is_word(const std::vector<std::string> &d, const char* word)
{
	return (strstr(d[iter(*word)].c_str(), word));
}

[[nodiscard]] ALWAYS_INLINE CONST const char *dict_is_word_nocase(const std::vector<std::string> &d, const char* word)
{
	return strstr(d[iter(*word)].c_str(), word) ?: strstr(d[iter_rev(*word)].c_str(), word);
}

int main(int argc, char **argv)
{
	std::vector<std::string> d;
	if (unlikely(!dict_load(&d))) {
		perror("alloc failed");
		return -1;
	}
	const char *ret = dict_is_word_nocase(d, WORD);
	if (!ret)
		return -1;
	while (*ret != '\n')
		putchar(*ret++);
	return 0;
}
