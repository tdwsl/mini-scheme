#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
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
	ins->depth = 0;
	return ins;
}

void freeInstance(Instance *ins) {
	freeToken(&ins->program);
	free(ins->functions);
	if(ins->variables)
		free(ins->variables);
	free(ins);
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

void simplifyArgs(Instance *ins, Token *args, int n) {
	for(int i = 0; i < n; i++) {
		if(args[i].type == LIST)
			args[i] = eval(ins, &args[i]);
		if(args[i].type == SYMBOL)
			args[i] = getVariable(ins, args[i].val.s);
	}
}

void runProgram(Instance *ins) {
	for(int i = 0; i < ins->program.num_children; i++)
		eval(ins, &ins->program.children[i]);
}

void loadString(Instance *ins, char *text) {
	getList(&ins->program, text);
	runProgram(ins);
}

void loadFile(Instance *ins, const char *filename) {
	int max = 200;
	char *str = malloc(max);
	int len = 0;

	FILE *fp = fopen(filename, "r");
	assert(fp);
	while(!feof(fp)) {
		str[len++] = fgetc(fp);
		if(len > max-30) {
			max += 70;
			str = realloc(str, max);
		}
	}
	str[len-1] = 0;
	fclose(fp);

	getList(&ins->program, str);
	free(str);
	
	runProgram(ins);
}

void defVariable(Instance *ins, char *s, Token t) {
	if(t.type == STRING) {
		char *ts = malloc(strlen(t.val.s)+1);
		strcpy(ts, t.val.s);
		t.val.s = ts;
	}

	for(int i = 0; i < ins->num_variables; i++)
		if(strcmp(ins->variables[i].s, s) == 0)
			if(ins->variables[i].depth == ins->depth) {
				freeToken(&ins->variables[i].token);
				ins->variables[i].token = t;
				return;
			}

	char *b = malloc(strlen(s)+1);
	strcpy(b, s);
	s = b;

	ins->variables = realloc(ins->variables,
			sizeof(Variable)*(++ins->num_variables));
	ins->variables[ins->num_variables-1] = (Variable){t, s, ins->depth};
}

void setVariable(Instance *ins, char *s, Token t) {
	if(t.type == STRING) {
		char *ts = malloc(strlen(t.val.s)+1);
		strcpy(ts, t.val.s);
		t.val.s = ts;
	}

	int depth = -1;
	int ind;
	for(int i = 0; i < ins->num_variables; i++)
		if(strcmp(ins->variables[i].s, s) == 0)
			if(ins->variables[i].depth > depth) {
				depth = ins->variables[i].depth;
				ind = i;
			}

	if(depth != -1) {
		freeToken(&ins->variables[ind].token);
		ins->variables[ind].token = t;
	}
	else {
		printf("error: could not set variable %s\n", s);
		exit(1);
	}
}

Token getVariable(Instance *ins, char *s) {
	int depth = -1;
	int ind;
	for(int i = 0; i < ins->num_variables; i++)
		if(strcmp(ins->variables[i].s, s) == 0)
			if(ins->variables[i].depth > depth) {
				depth = ins->variables[i].depth;
				ind = i;
			}
	if(depth != -1)
		return ins->variables[ind].token;

	printf("error: could not find variable %s\n", s);
	exit(1);
}

void cleanVariables(Instance *ins) {
	Variable *nu = 0;
	int nusz = 0;
	for(int i = 0; i < ins->num_variables; i++)
		if(ins->variables[i].depth <= ins->depth) {
			nu = realloc(nu, sizeof(Variable)*(++nusz));
			nu[nusz-1] = ins->variables[i];
			freeToken(&ins->variables[i].token);
			free(ins->variables[i].s);
		}
	free(ins->variables);
	ins->variables = 0;
	ins->num_variables = nusz;
	ins->variables = nu;
}
