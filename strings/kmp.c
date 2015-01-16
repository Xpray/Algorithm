#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
* get shift array of pattern
* return NULL if malloc call fails
*/

static int*
get_shift(char *pattern)
{
	size_t len = strlen(pattern);
	int *shift = malloc(sizeof(int) * len);
	
	if (shift == NULL) {
		return NULL;
	}
	
	shift[0] = -1;
	for(size_t i = 1; i < len; i++) {
		int j = shift[i - 1];
		while(j > -1 && pattern[j + 1] != pattern[i]) {
			j = shift[j];
		}
		
		if (pattern[j + 1] == pattern[i]) {
			j++;
		}
		shift[i] = j;
	}
	
	return shift;
}

/**
* return a positive number if match occurs,
* which points the position of the first matching char
* or -1 means no matching found
*/

int
kmp_match(char *context, char *pattern)
{
	int j = -1;
	int clen = strlen(context);
	int plen = strlen(pattern);
	
	int *shift = get_shift(pattern);
	if (shift == NULL) {
		return -1;
	}
	
	int key = -1;
	
	for(size_t i = 0; i < clen; i++) {
		while(j > -1 && pattern[j + 1] != context[i]) {
			j = shift[j]; 
		}
		if (pattern[j + 1] == context[i]) {
			j++;
		}
		
		if (j == plen - 1) {
			key = i - plen + 1;
			break;
		}
	}
	
	free(shift);
	return key;
}

int 
main(int argc, char **argv) 
{
	char *s = "abcbab";
	char *t = "bcbac";
	
	int key = kmp_match(s, t);
	
	printf("%d\n", key);
	
	return EXIT_SUCCESS;
}
