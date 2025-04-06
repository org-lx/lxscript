#pragma once

typedef enum {
	lxs_token_eof,
		
	lxs_token_identifier,
	lxs_token_number,
	lxs_token_string,

	lxs_token_lparen,
	lxs_token_rparen,
	lxs_token_lbrace,
	lxs_token_rbrace,
	lxs_token_semicolon,
	lxs_token_comma,

	lxs_token_assign,
	lxs_token_plus,
	lxs_token_minus,
	lxs_token_star,
	lxs_token_slash,
	lxs_token_eq,
	lxs_token_neq,
	lxs_token_lt,
	lxs_token_gt,
	lxs_token_leq,
	lxs_token_geq,

	lxs_token_if,
	lxs_token_else,
	lxs_token_while,
	lxs_token_return,
	lxs_token_function,
} lxs_token_type;

typedef struct {	
	lxs_token_type token_type;

	const char* start;
	int length;
} lxs_token;


// control
#define LXS_TOKEN_IF "if"
#define LXS_TOKEN_IF_SIZE 2
#define LXS_TOKEN_ELSE "else"
#define LXS_TOKEN_ELSE_SIZE 4
#define LXS_TOKEN_WHILE "while"
#define LXS_TOKEN_WHILE_SIZE 5
#define LXS_TOKEN_RETURN "return"
#define LXS_TOKEN_RETURN_SIZE 6
#define LXS_TOKEN_FUNCTION "func"
#define LXS_TOKEN_FUNCTION_SIZE 4

lxs_token_type lxs_check_keyword(const char* start, int length);
