#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "token.h"
#include "parse.h"

#define throwperror(E) { printf("error: %s\n", E); exit(1); }

void handleBraces(Token *list);

void subList(Token *list, int open, int close) {
	Token t;
	t.type = LIST;
	t.num_children = close-open-1;
	t.children = malloc(sizeof(Token)*t.num_children);
	for(int i = open+1; i <= close-1; i++)
		t.children[i-(open+1)] = list->children[i];

	for(int i = open+1; i <= list->num_children - (close-open); i++)
		list->children[i] = list->children[i-open+close];
	list->children[open] = t;
	handleBraces(&list->children[open]);
	list->num_children -= close-open;
}

void handleBraces(Token *list) {
	int open = -1, depth = 0;

	for(int i = 0; i < list->num_children; i++) {
		Token *t = &list->children[i];
		if(t->type != SYMBOL)
			continue;

		if(strcmp(t->val.s, "(") == 0) {
			if(depth == 0)
				open = i;
			depth++;
		}
		else if(strcmp(t->val.s, ")") == 0) {
			depth--;
			if(depth == 0) {
				subList(list, open, i);
				i = open;
			}
		}
	}
}

void addToken(Token *list, char type, char *str, int *len) {
	list->children = realloc(list->children,
			sizeof(Token)*(++list->num_children));
	Token *t = &list->children[list->num_children-1];

	t->type = type;

	t->val.s = malloc(*len+1);
	for(int i = 0; i < *len; i++)
		t->val.s[i] = str[i];
	t->val.s[*len] = 0;

	*len = 0;
}

void addChar(char **str, int *len, int *max, char c) {
	(*str)[(*len)++] = c;
	if((*len) > (*max) - 10) {
		(*max) += 20;
		(*str) = realloc(*str, *max);
	}
}

void debugWrite(Token *t, int ind) {
	if(t->type == LIST) {
		for(int i = 0; i < t->num_children; i++)
			debugWrite(&t->children[i], ind+1);
	}
	else {
		for(int i = 1; i < ind; i++)
			printf("  ");
		printf("%s\n", t->val.s);
	}
}

void getList(Token *list, char *text) {
	int max = 20;
	int len = 0;
	char *str = malloc(max);

	bool quote = false;
	bool comment = false;
	int depth = 0;

	for(char *c = text; *c; c++) {
		if(*c == '\n') {
			if(quote)
				throwperror("unterminated quote at eol");
			comment = false;
			if(len)
				addToken(list, SYMBOL, str, &len);
			continue;
		}

		if(comment)
			continue;

		if(*c == '"') {
			if(quote)
				addToken(list, STRING, str, &len);
			else if(len)
				addToken(list, SYMBOL, str, &len);
			quote = !quote;
			continue;
		}

		if(quote) {
			addChar(&str, &len, &max, *c);
			continue;
		}

		if(*c == ';') {
			if(len)
				addToken(list, SYMBOL, str, &len);
			comment = true;
			continue;
		}

		if(*c == '(' || *c == ')') {
			if(*c == '(')
				depth++;
			else if(*c == ')')
				depth--;
			if(depth < 0)
				throwperror("excess closing braces");

			if(len)
				addToken(list, SYMBOL, str, &len);
			addChar(&str, &len, &max, *c);
			addToken(list, SYMBOL, str, &len);
			continue;
		}

		if(*c == ' ' || *c == '\t') {
			if(len)
				addToken(list, SYMBOL, str, &len);
			continue;
		}

		addChar(&str, &len, &max, *c);
	}

	if(quote)
		throwperror("unterminated quote at eof");

	if(depth)
		throwperror("unterminated braces at eof");

	if(len)
		addToken(list, SYMBOL, str, &len);

	free(str);

	/*for(int i = 0; i < list->num_children; i++)
		printf("%s\n", list->children[i].val.s);*/

	handleBraces(list);

	/*debugWrite(list, 0);*/
}
