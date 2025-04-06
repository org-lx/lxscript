#include "lexer.h"
#include "str_utils.h"

void lxs_init_lexer(lxs_lexer* l, const char* source) {
	l->source = source;
	l->current = source;
}

char lxs_advance_lexer(lxs_lexer* l) {
	return *l->current++;
}

char lxs_peek_lexer(lxs_lexer* l) {
	return *l->current;
}

void lxs_skip_whitespace(lxs_lexer* l) {
	while (*l->current == ' ' || *l->current == '\t' || *l->current == '\n' || *l->current == '\r') {
		l->current++;
	}
}

lxs_token lxs_identifier(lxs_lexer* l) {
	const char* start = l->current - 1;
	while (lxs_is_alpha(lxs_peek_lexer(l)) || lxs_is_digit(lxs_peek_lexer(l))) {
		lxs_advance_lexer(l);
	}
	int length = (int)(l->current-start);
	
	return (lxs_token) { lxs_check_keyword(start, length), start, length };
}

lxs_token lxs_number(lxs_lexer* l) {
	const char* start = l->current-1;
	while (lxs_is_digit(lxs_peek_lexer(l))) {
		lxs_advance_lexer(l);
	}

	return (lxs_token){lxs_token_number, start, (int)(l->current-start)};
}

lxs_token lxs_string(lxs_lexer* l) {
    const char* start = l->current;
    while (lxs_peek_lexer(l) != '"' && lxs_peek_lexer(l) != '\0') { 
		 lxs_advance_lexer(l);
	 }
    if (lxs_peek_lexer(l) == '"') lxs_advance_lexer(l);
    return (lxs_token){ lxs_token_string, start, (int)(l->current - start - 1) };
}

lxs_token single_char_token(lxs_lexer* l, lxs_token_type type) {
	return (lxs_token) { type, l->current-1, 1};
}

lxs_token lxs_next_token(lxs_lexer* l) {
	lxs_skip_whitespace(l);

	if (*l->current == '\0') {
		return single_char_token(l, lxs_token_eof);
	}

	char c = lxs_advance_lexer(l);

	if (lxs_is_alpha(c)) return lxs_identifier(l);
	if (lxs_is_digit(c)) return lxs_number(l);

	switch (c) {
        case '(': return single_char_token(l, lxs_token_lparen);
        case ')': return single_char_token(l, lxs_token_rparen);
        case '{': return single_char_token(l, lxs_token_lbrace);
        case '}': return single_char_token(l, lxs_token_rbrace);
        case ';': return single_char_token(l, lxs_token_semicolon);
        case ',': return single_char_token(l, lxs_token_comma);
        case '=': return (*l->current == '=') ? (lxs_advance_lexer(l), (lxs_token){ lxs_token_eq, l->current - 2, 2 }) : single_char_token(l, lxs_token_assign);
        case '!': return (*l->current == '=') ? (lxs_advance_lexer(l), (lxs_token){ lxs_token_neq, l->current - 2, 2 }) : single_char_token(l, lxs_token_assign);
        case '<': return (*l->current == '=') ? (lxs_advance_lexer(l), (lxs_token){ lxs_token_leq, l->current - 2, 2 }) : single_char_token(l, lxs_token_lt);
        case '>': return (*l->current == '=') ? (lxs_advance_lexer(l), (lxs_token){ lxs_token_geq, l->current - 2, 2 }) : single_char_token(l, lxs_token_gt);
        case '+': return single_char_token(l, lxs_token_plus);
        case '-': return single_char_token(l, lxs_token_minus);
        case '*': return single_char_token(l, lxs_token_star);
        case '/': return single_char_token(l, lxs_token_slash);
        case '"': return lxs_string(l);
    }

	return single_char_token(l, lxs_token_eof);
}
