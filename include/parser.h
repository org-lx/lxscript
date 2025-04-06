#pragma once

#include "lexer.h"
#include "ast_node.h"

typedef struct {
	lxs_lexer* lexer;
	lxs_token current;
} lxs_parser;

void lxs_init_parser(lxs_parser*, lxs_lexer*);
lxs_token lxs_consume_parser(lxs_parser*, lxs_token_type);

lxs_ast_node* lxs_parse_literal(lxs_parser*);
lxs_ast_node* lxs_parse_identifier(lxs_parser*);
lxs_ast_node* lxs_parse_expression(lxs_parser*);
lxs_ast_node* lxs_parse_term(lxs_parser*);
lxs_ast_node* lxs_parse_binary_op(lxs_parser*);
lxs_ast_node* lxs_parse_assignment(lxs_parser*);
lxs_ast_node* lxs_parse_if(lxs_parser*);
lxs_ast_node* lxs_parse_while(lxs_parser*);
lxs_ast_node* lxs_parse_function(lxs_parser*);
lxs_ast_node* lxs_parse_return(lxs_parser*);
lxs_ast_node* lxs_parse_statement(lxs_parser*);
lxs_ast_node* lxs_parse_program(lxs_parser*);
