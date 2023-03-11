#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "/home/james/c/nix.c/nix.h"

#define filename argv[1]

int main(int argc, char **argv)
{
	char *buf;
	if (unlikely(!nix_cat_auto(&buf, filename)))
		return -1;
	for (;;) {
		if (unlikely(!(buf = strstr(buf, "/wiki/"))))
			return -1;
		buf += strlen("/wiki/");
		for (;;) {
			switch (*buf) {
			default:
				putchar(*buf++);
				continue;
			case '#':
			case '<':
				putchar('\n');
			}
			break;
		}
	}
	free(buf);
	return 0;
}
