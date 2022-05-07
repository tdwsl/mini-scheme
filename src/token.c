#include <stdlib.h>
#include <stdio.h>
#include "token.h"

void freeToken(Token *t) {
	if(t->type == STRING || t->type == SYMBOL) {
		/*printf("freeing %s (0x%x)\n", t->val.s, t->val.s);*/
		free(t->val.s);
	}
	else if(t->type == LIST) {
		for(int i = 0; i < t->num_children; i++)
			freeToken(&t->children[i]);
		free(t->children);
	}
	else if(t->type == FUNCTION) {
		for(int i = 0; i < t->num_variables; i++)
			free(t->variables[i]);
		free(t->variables);
	}
}
