#ifndef PARSER_H_
#define PARSER_H_

#include <list>
#include "utils.h"
#include "parsetree.h"

class Parser
{
private:
	std::list<Token> content;
	std::list<Token>::const_iterator pos; // use iterator as token pointer

	// below are TreeNode generators (corresponding to grammar rules)
	// plan to use exception handling to report errors
	TreeNode program();
public:
	Parser(std::list<Token> tokenList);

	TreeNode parse();
};

#endif