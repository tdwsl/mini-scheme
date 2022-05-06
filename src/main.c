#include "instance.h"
#include "scheme.h"

int main() {
	Instance *ins = newInstance();
	addSchemeFunctions(ins);
	loadFile(ins, "test.lisp");
	freeInstance(ins);
	return 0;
}
