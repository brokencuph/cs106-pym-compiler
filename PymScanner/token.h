#ifndef TOKENLIST_H_
#define TOKENLIST_H_

#include <string>

enum class TokenType // use enum class instead of enum to avoid name collision
{
	NUMBER, CHARS, // Literals
	PLUS, MINUS, MUL, DIV, MOD, // math operators
	LPR, RPR, LBR, RBR, // delimiters
	ASSI, // assignment
	GT, LT, GE, LE, EQ, NEQ, // relational operators
	AND, OR, NOT, // logical operators
	INDENT, DEDENT, // tokens relevant to indentation
	NEWLINE, FEOF, // controling tokens
	ERROR // error token
};

struct Token
{
	TokenType type;
	std::string str; // use std::string as the character storage
	int line;

	Token(TokenType _type, std::string _str, int line);
};

#endif