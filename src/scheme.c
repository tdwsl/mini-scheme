#include <stdio.h>
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
	assert(args[0].type == LIST);

	ins->depth++;

	int sz = args[0].num_children;
	Token *ass = args[0].children;
	for(int i = 0; i < sz; i++) {
		assert(ass[i].type == LIST);
		assert(ass[i].num_children == 2);
		assert(ass[i].children[0].type == SYMBOL);
		if(ass[i].children[1].type == LIST)
			ass[i].children[1] = eval(ins, &ass[i].children[1]);

		defVariable(ins, ass[i].children->val.s, ass[i].children[1]);
	}

	Token t = nilToken();
	for(int i = 1; i < n; i++)
		t = eval(ins, &args[i]);

	ins->depth--;
	cleanVariables(ins);
	return t;
}

Token fSetf(Instance *ins, Token *args, int n) {
	assert(n == 2);
	assert(args[0].type == SYMBOL);
	simplifyArgs(ins, args+1, 1);

	setVariable(ins, args[0].val.s, args[1]);
	return args[1];
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
}
