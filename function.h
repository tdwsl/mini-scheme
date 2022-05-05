#ifndef FUNCTION_H
#define FUNCTION_H

#include "token.h"
#include "instance.h"

struct instance;
typedef struct instance Instance;

typedef Token (*cfun)(struct instance*, Token*, int);

typedef struct function {
	cfun fun;
	const char *s;
} Function;

void addFunction(Instance *ins, cfun fun, const char *s);
Token doFunction(Instance *ins, const char *s, Token *args, int n);

#endif
