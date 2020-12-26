#include <utility>
#include <string>
#include <iostream>
//#include <unordered_map>
#include "token.h"


static std::string tokenNames[42] = { "ID","NUMBER","CHARS", // Literals
	"PLUS","MINUS","MUL","DIV","MOD", // math operators
	"LPR","RPR","LBR","RBR", // delimiters
	"ASSI", // assignment
	"GT","LT","GE","LE","EQ","NEQ", // relational operators
	"INDENT","DEDENT", // tokens relevant to indentation
	"NEWLINE","FEOF", // controling tokens
	"ERROR", // error token
	"COMMA","COLON",//"," ;
	"IF","ELSE","ELIF","WHILE","DEF","RETURN","AND","OR","NOT","INT","NUM","STR",//keywords
	"ARROW" };

Token::Token(TokenType _type, std::string _str, int _line) : type(_type), str(std::move(_str)), line(_line)
{

}

std::ostream& operator<<(std::ostream& os, const Token& tk)
{
	return os << tk.line << ' ' << tokenNames[(int)tk.type] << ' ' << tk.str << '\n';
}
