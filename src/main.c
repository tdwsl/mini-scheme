#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "instance.h"
#include "scheme.h"
#include "token.h"

int main(int argc, char **args) {
	assert(argc <= 2);
	Instance *ins = newInstance();
	addSchemeFunctions(ins);

	if(argc > 1)
		loadFile(ins, args[1]);
	else {
		printf("Scheme interpreter - tdwsl 2022\n");
		int max = 50;
		char *s = malloc(max);
		int len = 0;
		while(!ins->quit) {
			printf(">");
			for(scanf("%c", &s[len++]); s[len-1] != '\n';
					scanf("%c", &s[len++]))
				if(len > max-20) {
					max += 40;
					s = realloc(s, max);
				}
			s[len-1] = 0;
			loadString(ins, s);
			freeToken(&ins->program);
			ins->program = newList();
			len = 0;
		}
		free(s);
	}

	freeInstance(ins);
	return 0;
}
