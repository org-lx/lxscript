#include "str_utils.h"

#include <stdlib.h>

int lxs_is_alpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

int lxs_is_digit(char c) {
	return c >= '0' && c <= '9';
}

char* lxs_strndup(char *str, int chars) {
    char *buffer;
    int n;

    buffer = (char*)malloc(chars +1);
    if (buffer) {
        for (n = 0; ((n < chars) && (str[n] != 0)); n++) buffer[n] = str[n];
        buffer[n] = 0;
    }

    return buffer;
}
