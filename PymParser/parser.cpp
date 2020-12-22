#include <stdexcept>
#include <memory>

#include "parser.h"

using std::make_shared;

Parser::Parser(std::list<Token> tokenList) : content(std::move(tokenList)), pos(content.cbegin())
{
	
}

SharedTreeNode Parser::parse()
{
	pos = content.cbegin();
	return program();
}

SharedTreeNode Parser::program()
{
	auto root = statement_list();
	if (pos->type != TokenType::FEOF)
	{
		throw std::invalid_argument("Internal error: no EOF token found.");
	}
	return root;
}

SharedTreeNode Parser::statement_list()
{
	auto root = make_shared<TreeNode>();
	auto curStmtPos = &root->children[0]; // address of write position
	while (pos != content.cend() && pos->type != TokenType::FEOF)
	{
		auto nextPos = ++std::list<Token>::const_iterator(pos);
		auto stmt = [this, nextPos]() -> SharedTreeNode {
			switch (pos->type)
			{
			case TokenType::DEF:
				return def_stmt();
			case TokenType::IF:
				return if_stmt();
			case TokenType::WHILE:
				return while_stmt();
			case TokenType::RETURN:
				return return_stmt();
			case TokenType::INDENT:
				return compound_stmt();
			case TokenType::ID:
				if (nextPos != content.cend() && nextPos->type == TokenType::COLON)
				{
					return decl_stmt();
				}
				else
				{
					return expr_stmt();
				}
			default:
				return expr_stmt();
			}
		};
		*curStmtPos = stmt();
		curStmtPos = &((*curStmtPos)->rSibling);
		pos++;
	}
}

SharedTreeNode Parser::statement()
{

}