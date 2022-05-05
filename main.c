#include <stdio.h>
#include <assert.h>
#include "instance.h"
#include "function.h"

Token fDisplay(Instance *ins, Token *args, int n) {
	assert(n == 1);

	switch(args[0].type) {
	case STRING:
		printf("%s", args[0].val.s);
		break;
	case INTEGER:
		printf("%d", args[0].val.i);
		break;
	case FLOAT:
		printf("%f", args[0].val.f);
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
int main() {
	Instance *ins = newInstance();
	addFunction(ins, fDisplay, "display");
	addFunction(ins, fNewline, "newline");
	/*loadString(ins, "(setq num (+ 47 1)) (write-line (getf hola))");*/
	loadString(ins, "(display \"Hello world!\") (newline)");
	runProgram(ins);
	freeInstance(ins);
	return 0;
}
