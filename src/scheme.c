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

void addSchemeFunctions(Instance *ins) {
	addFunction(ins, fDisplay, "display");
	addFunction(ins, fNewline, "newline");
	addFunction(ins, fPlus, "+");
	addFunction(ins, fMinus, "-");
	addFunction(ins, fMultiply, "*");
	addFunction(ins, fDivide, "/");
}
