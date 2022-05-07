#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "token.h"

void freeToken(Token *t) {
	if(t->type == STRING || t->type == SYMBOL) {
		/*printf("freeing %s (0x%x)\n", t->val.s, t->val.s);*/
		free(t->val.s);
	}
	else if(t->type == LIST || t->type == VECTOR) {
		for(int i = 0; i < t->num_children; i++)
			freeToken(&t->children[i]);
		free(t->children);
		t->num_children = 0;
	}
	else if(t->type == FUNCTION) {
		for(int i = 0; i < t->num_variables; i++)
			free(t->variables[i]);
		free(t->variables);
	}
}

void seperateToken(Token *t) {
	if(t->type == STRING) {
		char *s = malloc(strlen(t->val.s)+1);
		strcpy(s, t->val.s);
		t->val.s = s;
	}
	else if(t->type == LIST || t->type == VECTOR) {
		if(t->num_children == 0)
			return;
		Token *children = malloc(
				t->num_children*sizeof(Token));
		for(int i = 0; i < t->num_children; i++) {
			children[i] = t->children[i];
			seperateToken(&children[i]);
		}
		t->children = children;
	}
}
