#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "function.h"
#include "token.h"
#include "instance.h"

Token doFunction(Instance *ins, const char *s, Token *args, int n) {
	for(int i = 0; i < ins->num_functions; i++)
		if(strcmp(ins->functions[i].s, s) == 0)
			return ins->functions[i].fun(ins, args, n);
	printf("error: could not find function %s\n", s);
	exit(1);
}

void addFunction(Instance *ins, cfun fun, const char *s) {
	ins->functions = realloc(ins->functions,
			sizeof(Function)*(++ins->num_functions));
	ins->functions[ins->num_functions-1] = (Function){fun, s};
}
