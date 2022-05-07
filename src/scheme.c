#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "instance.h"
#include "function.h"
#include "token.h"

Token fDisplay(Instance *ins, Token *args, int n) {
	assert(n == 1);

	simplifyArgs(ins, args, n);

	switch(args[0].type) {
	case STRING:
		printf("%s", args[0].val.s);
		break;
	case INTEGER:
		printf("%d", args[0].val.i);
		break;
	case FLOAT:
		printf("%g", args[0].val.f);
		break;
	case CHAR:
		printf("%c", args[0].val.c);
		break;
	case HASHT:
		printf("#t");
		break;
	case NIL:
		printf("#f");
		break;
	default:
		printf("(?)");
		break;
	}

	return nilToken();
}

Token fNewline(Instance *ins, Token *args, int n) {
	assert(n == 0);
	printf("\n");
	return nilToken();
}

defArithmeticFun(+, fPlus);
defArithmeticFun(-, fMinus);
defArithmeticFun(*, fMultiply);
defArithmeticFun(/, fDivide);

Token fLet(Instance *ins, Token *args, int n) {
	assert(args[0].type == LIST || args[0].type == SYMBOL);

	bool sym = args[0].type == SYMBOL;
	char **variables;
	Token *vals;
	if(sym) {
		variables = malloc(sizeof(char*)*args[1].num_children);
		vals = malloc(sizeof(Token)*args[1].num_children);
		args++;
	}
	else
		ins->depth++;

	int sz = args[0].num_children;
	Token *ass = args[0].children;
	for(int i = 0; i < sz; i++) {
		assert(ass[i].type == LIST);
		assert(ass[i].num_children == 2);
		assert(ass[i].children[0].type == SYMBOL);
		if(ass[i].children[1].type == LIST)
			ass[i].children[1] = eval(ins, &ass[i].children[1]);

		if(sym) {
			variables[i] = ass[i].children[0].val.s;
			vals[i] = ass[i].children[1];
		}
		else
			defVariable(ins, ass[i].children[0].val.s,
					ass[i].children[1]);
	}

	if(sym) {
		ins->depth++;
		addVarFunction(ins, (args-1)[0].val.s, args+1, n-2,
				variables, args[0].num_children);
		Token t = doFunction(ins, (args-1)[0].val.s,
				vals, args[0].num_children);
		ins->depth--;
		free(vals);
		free(variables);
		cleanVariables(ins);
		return t;
	}
	else {
		Token t = nilToken();
		for(int i = 1; i < n; i++)
			t = eval(ins, &args[i]);

		ins->depth--;
		cleanVariables(ins);
		return t;
	}
}

Token fSetf(Instance *ins, Token *args, int n) {
	assert(n == 2);
	assert(args[0].type == SYMBOL);
	simplifyArgs(ins, args+1, 1);

	setVariable(ins, args[0].val.s, args[1]);
	return args[1];
}

Token fBegin(Instance *ins, Token *args, int n) {
	Token t;
	for(int i = 0; i < n; i++)
		t = eval(ins, &args[i]);
	return t;
}

Token fIf(Instance *ins, Token *args, int n) {
	assert(n <= 3 && n > 1);
	for(int i = 1; i < n; i++)
		assert(args[i].type == LIST);

	Token t = eval(ins, &args[0]);
	if(t.type == HASHT)
		return eval(ins, &args[1]);
	else if(t.type == NIL) {
		if(n > 2)
			return eval(ins, &args[2]);
		else
			return nilToken();
	}
	else {
		printf("error: if has non boolean value\n");
		exit(1);
	}
}

void getAB(Token *args, float *a, float *b) {
	if(args[0].type == INTEGER)
		*a = args[0].val.i;
	else if(args[0].type == FLOAT)
		*a = args[0].val.f;
	else
		*a = args[0].val.c;

	if(args[1].type == INTEGER)
		*b = args[1].val.i;
	else if(args[1].type == FLOAT)
		*b = args[1].val.f;
	else
		*b = args[1].val.c;
}

Token fEquals(Instance *ins, Token *args, int n) {
	assert(n == 2);
	simplifyArgs(ins, args, n);
	assert(args[0].type != STRING && args[1].type != STRING);

	if((args[0].type == INTEGER || args[0].type == FLOAT) &&
			(args[1].type == INTEGER || args[1].type == FLOAT)) {
		float a, b;
		getAB(args, &a, &b);
		if(a == b)
			return trueToken();
		else
			return nilToken();
	}
	else if(args[0].type != args[0].type)
		return nilToken();
	else if(args[0].type == NIL || args[0].type == HASHT)
		return trueToken();
	else if(args[0].type == CHAR)
		return (args[0].val.c == args[1].val.c) ?
			trueToken() : nilToken();
	else
		return nilToken();
}

Token fStringEquals(Instance *ins, Token *args, int n) {
	assert(n == 2);
	simplifyArgs(ins, args, n);
	assert(args[0].type == STRING && args[1].type == STRING);

	return (strcmp(args[0].val.s, args[1].val.s) == 0) ?
		trueToken() : nilToken();
}

Token fGreater(Instance *ins, Token *args, int n) {
	assert(n == 2);
	simplifyArgs(ins, args, n);

	if(args[0].type != INTEGER && args[0].type != FLOAT
			&& args[0].type != CHAR)
		return nilToken();
	if(args[1].type != INTEGER && args[1].type != FLOAT
			&& args[1].type != CHAR)
		return nilToken();

	float a, b;
	getAB(args, &a, &b);

	if(a > b)
		return trueToken();
	else
		return nilToken();
}

Token fLess(Instance *ins, Token *args, int n) {
	assert(n == 2);
	simplifyArgs(ins, args, n);

	if(args[0].type != INTEGER && args[0].type != FLOAT
			&& args[0].type != CHAR)
		return nilToken();
	if(args[1].type != INTEGER && args[1].type != FLOAT
			&& args[1].type != CHAR)
		return nilToken();

	float a, b;
	getAB(args, &a, &b);

	if(a < b)
		return trueToken();
	else
		return nilToken();
}

Token fGreaterEq(Instance *ins, Token *args, int n) {
	assert(n == 2);
	simplifyArgs(ins, args, n);

	if(args[0].type != INTEGER && args[0].type != FLOAT
			&& args[0].type != CHAR)
		return nilToken();
	if(args[1].type != INTEGER && args[1].type != FLOAT
			&& args[1].type != CHAR)
		return nilToken();

	float a, b;
	getAB(args, &a, &b);

	if(a >= b)
		return trueToken();
	else
		return nilToken();
}

Token fLessEq(Instance *ins, Token *args, int n) {
	assert(n == 2);
	simplifyArgs(ins, args, n);

	if(args[0].type != INTEGER && args[0].type != FLOAT
			&& args[0].type != CHAR)
		return nilToken();
	if(args[1].type != INTEGER && args[1].type != FLOAT
			&& args[1].type != CHAR)
		return nilToken();

	float a, b;
	getAB(args, &a, &b);

	if(a <= b)
		return trueToken();
	else
		return nilToken();
}

Token fDefine(Instance *ins, Token *args, int n) {
	if(args[0].type == LIST) {
		assert(args[0].type == LIST);
		for(int i = 0; i < args[0].num_children; i++)
			assert(args[0].children[i].type == SYMBOL);

		int num_variables = args[0].num_children-1;
		char **variables = malloc(sizeof(char*)*num_variables);

		for(int i = 0; i < num_variables; i++)
			variables[i] = args[0].children[i+1].val.s;

		addVarFunction(ins, args[0].children[0].val.s, args+1, n-1,
				variables, num_variables);

		free(variables);
		return getVariable(ins, args[0].children[0].val.s);
	}
	else {
		assert(args[0].type == SYMBOL);
		assert(n == 2);
		simplifyArgs(ins, args+1, 1);
		defVariable(ins, args[0].val.s, args[1]);
		return args[1];
	}
}

Token fQuit(Instance *ins, Token *args, int n) {
	ins->quit = true;
	return nilToken();
}

Token fRead(Instance *ins, Token *args, int n) {
	assert(n == 0);

	int max = 30;
	int len = 0;
	char *s = malloc(max);
	for(scanf("%c", &s[len++]); s[len-1] != '\n'; scanf("%c", &s[len++])) {
		if(len > max-20) {
			max += 50;
			s = realloc(s, max);
		}
	}
	s[len-1] = 0;

	Token t;
	t.type = STRING;
	t.val.s = s;
	return t;
}

Token fStringRef(Instance *ins, Token *args, int n) {
	assert(n == 2);
	simplifyArgs(ins, args, n);
	assert(args[0].type == STRING);
	assert(args[1].type == INTEGER);

	assert(args[1].val.i < strlen(args[0].val.s) && args[1].val.i >= 0);
	return newChar(args[0].val.s[args[1].val.i]);
}

Token fVector(Instance *ins, Token *args, int n) {
	simplifyArgs(ins, args, n);

	Token t;
	t.type = VECTOR;
	t.children = malloc(sizeof(Token)*n);
	for(int i = 0; i < n; i++) {
		t.children[i] = args[i];
		seperateToken(&t.children[i]);
	}
	t.num_children = n;
	return t;
}

Token fVectorRef(Instance *ins, Token *args, int n) {
	assert(n == 2);
	simplifyArgs(ins, args, n);
	assert(args[0].type == VECTOR);
	assert(args[1].type == INTEGER);
	assert(args[1].val.i >= 0 && args[1].val.i < args[0].num_children);
	Token t = args[0].children[args[1].val.i];
	return t;
}

Token fLength(Instance *ins, Token *args, int n) {
	assert(n == 1);
	simplifyArgs(ins, args, n);
	assert(args[0].type == VECTOR || args[0].type == STRING);
	if(args[0].type == VECTOR)
		return newInt(args[0].num_children);
	else
		return newInt(strlen(args[0].val.s));
}

void addSchemeFunctions(Instance *ins) {
	addFunction(ins, fDisplay, "display");
	addFunction(ins, fNewline, "newline");
	addFunction(ins, fPlus, "+");
	addFunction(ins, fMinus, "-");
	addFunction(ins, fMultiply, "*");
	addFunction(ins, fDivide, "/");
	addFunction(ins, fLet, "let");
	addFunction(ins, fSetf, "set!");
	addFunction(ins, fBegin, "begin");
	addFunction(ins, fIf, "if");
	addFunction(ins, fEquals, "=");
	addFunction(ins, fGreater, ">");
	addFunction(ins, fGreaterEq, ">=");
	addFunction(ins, fLess, "<");
	addFunction(ins, fLessEq, "<=");
	addFunction(ins, fDefine, "define");
	addFunction(ins, fQuit, "exit");
	addFunction(ins, fRead, "read");
	addFunction(ins, fStringEquals, "string=?");
	addFunction(ins, fStringRef, "string-ref");
	addFunction(ins, fVector, "vector");
	addFunction(ins, fVectorRef, "vector-ref");
	addFunction(ins, fLength, "length");
}
