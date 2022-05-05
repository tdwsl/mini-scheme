#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "function.h"
#include "token.h"
#include "instance.h"
#include "parse.h"

Instance *newInstance() {
	Instance *ins = malloc(sizeof(Instance));
	ins->functions = 0;
	ins->num_functions = 0;
	ins->variables = 0;
	ins->num_functions = 0;
	ins->program = newList();
	return ins;
}

void freeInstance(Instance *ins) {
	freeToken(&ins->program);
	free(ins->functions);
	free(ins);
}

void loadString(Instance *ins, char *text) {
	getList(&ins->program, text);
}

Token eval(Instance *ins, Token *t) {
	if(t->type != LIST) {
		printf("error: cannot evaluate non-list\n");
		exit(1);
	}

	if(t->children[0].type != SYMBOL) {
		printf("error: must be a symbol\n");
		exit(1);
	}

	int sz = t->num_children-1;
	Token *tokens = malloc(sizeof(Token)*sz);
	for(int i = 0; i < sz; i++)
		tokens[i] = t->children[i+1];

	/*for(int i = 0; i < sz; i++)
		if(tokens[i].type == SYMBOL)
			tokens[i] = eval(ins, getVariable(tokens[i].s));
	*/

	Token k = doFunction(ins, t->children[0].val.s, tokens, sz);

	for(int i = 0; i < sz; i++)
		if(tokens[i].type != t->children[i+1].type)
			freeToken(&tokens[i]);
	free(tokens);
	return k;
}

void runProgram(Instance *ins) {
	for(int i = 0; i < ins->program.num_children; i++)
		eval(ins, &ins->program.children[i]);
}
