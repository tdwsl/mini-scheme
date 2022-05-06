#include <stdlib.h>
#include "token.h"

void freeToken(Token *t) {
	if(t->type == STRING || t->type == SYMBOL)
		free(t->val.s);
	else if(t->type == LIST) {
		for(int i = 0; i < t->num_children; i++)
			freeToken(&t->children[i]);
	}
}
