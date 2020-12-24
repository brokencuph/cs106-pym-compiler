#ifndef PARSER_H_
#define PARSER_H_

#include <list>
#include "utils.h"
#include "parsetree.h"

using SharedTreeNode = std::shared_ptr<TreeNode>;

class Parser
{
private:
	std::list<Token> content;
	std::list<Token>::const_iterator pos; // use iterator as token pointer

	// below are TreeNode generators (corresponding to grammar rules)
	// plan to use exception handling to report errors
	SharedTreeNode program();

	SharedTreeNode statement_list();

	SharedTreeNode statement();

	SharedTreeNode def_stmt();

	SharedTreeNode decl_stmt();

	SharedTreeNode return_stmt();

	SharedTreeNode expr_stmt();

	SharedTreeNode if_stmt();

	SharedTreeNode while_stmt();

	SharedTreeNode compound_stmt();

	SharedTreeNode param_list();

	SharedTreeNode param();

	SharedTreeNode expression();

	SharedTreeNode or_expr();

	SharedTreeNode and_not_expr();

	SharedTreeNode not_expr();

	SharedTreeNode relational_expr();

	SharedTreeNode add_min_expr();

	SharedTreeNode mul_div_expr();

	SharedTreeNode sign_expr();

	SharedTreeNode factor();

	SharedTreeNode arg_list();

	std::pair<TokenType, bool> interpret_type();
public:
	Parser(std::list<Token> tokenList);

	SharedTreeNode parse();
};

#endif