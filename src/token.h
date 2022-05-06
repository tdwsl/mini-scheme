#ifndef TOKEN_H
#define TOKEN_H

enum {
	STRING,
	INTEGER,
	FLOAT,
	CHAR,
	LIST,
	SYMBOL,
	NIL,
};

typedef struct token {
	char type;

	union {
		int i;
		float f;
		char *s;
		char c;
	} val;

	struct token *children;
	int num_children;
} Token;

#define newList() (Token) { LIST, {0}, 0, 0 }
#define nilToken() (Token) { NIL, {0}, 0, 0 }
#define newInt(I) (Token) { INTEGER, {(int)I}, 0, 0 }
#define newFloat(F) (Token) { FLOAT, {.f=F}, 0, 0 }
void freeToken(Token *t);

#endif
