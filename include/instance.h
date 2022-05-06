#ifndef INSTANCE_H
#define INSTANCE_H

#include <stdbool.h>
#include "token.h"
#include "function.h"

typedef struct variable {
	Token token;
	char *s;
	int depth;
} Variable;

typedef struct instance {
	Token program;
	struct function *functions;
	int num_functions;
	struct variable *variables;
	int num_variables;
	int depth;
} Instance;

#define defArithmeticFun(OP, NAME)\
Token NAME(Instance *ins, Token *args, int n) {\
	assert(n == 2);\
	simplifyArgs(ins, args, n);\
	assert(args[0].type == FLOAT || args[0].type == INTEGER);\
	assert(args[1].type == FLOAT || args[1].type == INTEGER);\
	float t;\
	if(args[0].type == INTEGER)\
		t = args[0].val.i;\
	else\
		t = args[0].val.f;\
	if(args[1].type == INTEGER)\
		t = t OP args[1].val.i;\
	else\
		t = t OP args[1].val.f;\
	if(args[0].type == INTEGER && args[1].type == INTEGER\
			&& (float)(int)t == t)\
		return newInt(t);\
	else\
		return newFloat(t);\
}

void freeInstance(Instance *ins);
Instance *newInstance();
void loadString(Instance *ins, char *text);
void loadFile(Instance *ins, const char *filename);
Token eval(Instance *ins, Token *t);
void simplifyArgs(Instance *ins, Token *args, int n);
void defVariable(Instance *ins, char *s, Token t);
void setVariable(Instance *ins, char *s, Token t);
Token getVariable(Instance *ins, char *s);
void cleanVariables(Instance *ins);
Token callVariable(Instance *ins, char *s, Token *args, int n);
bool isVariable(Instance *ins, char *s);
void addVarFunction(Instance *ins, char *s, Token *body, int n,
		char **variables, int num_variables);

#endif
