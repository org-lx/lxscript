#include "token.h"
#include "lexer.h"

#include <string.h>

#define CHECK_KEYWORD(l, v, ret) \
	if (length == l && strncmp(start, v, l) == 0) return ret;

lxs_token_type lxs_check_keyword(const char* start, int length) {
	CHECK_KEYWORD(LXS_TOKEN_IF_SIZE, LXS_TOKEN_IF, lxs_token_if);
	CHECK_KEYWORD(LXS_TOKEN_ELSE_SIZE, LXS_TOKEN_ELSE, lxs_token_else);
	CHECK_KEYWORD(LXS_TOKEN_WHILE_SIZE, LXS_TOKEN_WHILE, lxs_token_while);
	CHECK_KEYWORD(LXS_TOKEN_RETURN_SIZE, LXS_TOKEN_RETURN, lxs_token_return);
	CHECK_KEYWORD(LXS_TOKEN_FUNCTION_SIZE, LXS_TOKEN_FUNCTION, lxs_token_function);
	return lxs_token_identifier;
}
