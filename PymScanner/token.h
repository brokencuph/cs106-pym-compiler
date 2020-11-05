#ifndef TOKENLIST_H_
#define TOKENLIST_H_
#include <string>

enum class TokenType // use enum class instead of enum to avoid name collision
{
	ID,
	NUMBER, CHARS, // Literals
	PLUS, MINUS, MUL, DIV, MOD, // math operators
	LPR, RPR, LBR, RBR, // delimiters
	ASSI, // assignment
	GT, LT, GE, LE, EQ, NEQ, // relational operators
	INDENT, DEDENT, // tokens relevant to indentation
	NEWLINE, FEOF, // controling tokens
	ERROR, // error token
	COMMA, COLON,//, ;
	IF, ELSE, ELIF, WHILE, DEF, RETURN, AND, OR, NOT,INT, NUM, STR,//keywords
	ARROW
};

struct Token
{
	TokenType type;
	std::string str; // use std::string as the character storage
	int line;

	Token(TokenType _type, std::string _str, int line);

	friend std::ostream& operator<<(std::ostream& os, const Token& tk);
};

#endif