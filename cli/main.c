#include <stdio.h>

#include "parser.h"
#include "context.h"

void print_token(lxs_token token) {
    printf("Token: Type=%d, Text=\"%.*s\"\n", token.token_type, token.length, token.start);
}

int main(void) {
	lxs_context ctx = {0};
	const char* test_source = 
		"x = 6;"
		"y = x + 5;"
		"return x;";

	lxs_lexer lexer;
	lxs_init_lexer(&lexer, test_source);

	lxs_parser parser;
	lxs_init_parser(&parser, &lexer);

	lxs_ast_node* program = lxs_parse_program(&parser);
	
	for (int i = 0; i < program->body_count; ++i) {
		lxs_print_ast_node(program->body[i], 0);
	}
	lxs_execute_program(&ctx, program);


   return 0;
}
