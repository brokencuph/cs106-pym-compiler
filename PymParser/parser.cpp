#include <stdexcept>

#include "parser.h"

Parser::Parser(std::list<Token> tokenList) : content(std::move(tokenList)), pos(tokenList.cbegin())
{
	
}

TreeNode Parser::parse()
{
	return program();
}

TreeNode Parser::program()
{
	TreeNode root;
	while (pos->type != TokenType::FEOF)
	{

	}
}