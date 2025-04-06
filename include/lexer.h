#pragma once

#include "token.h"

typedef struct {
	const char* source;
	const char* current;
} lxs_lexer;

void lxs_init_lexer(lxs_lexer*, const char* source);
lxs_token lxs_next_token(lxs_lexer*);

lxs_token lxs_identifier(lxs_lexer*);
lxs_token lxs_number(lxs_lexer*);
lxs_token lxs_string(lxs_lexer*);

char lxs_advance_lexer(lxs_lexer*);
char lxs_peek_lexer(lxs_lexer*);

void lxs_skip_whitespace(lxs_lexer*);
