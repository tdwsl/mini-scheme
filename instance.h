#ifndef INSTANCE_H
#define INSTANCE_H

#include "token.h"
#include "function.h"

typedef struct variable {
	Token token;
	char *identifier;
} Variable;

typedef struct instance {
	Token program;
	struct function *functions;
	int num_functions;
	struct variable *variables;
	int num_variables;
} Instance;

void freeInstance(Instance *ins);
Instance *newInstance();
void loadString(Instance *ins, char *text);

void runProgram(Instance *ins);

#endif
