#include <iostream>
#include "scanner.h"
#include "token.h"
#include "utils.h"
#include "dfa.h"


using namespace std::string_literals;

bool g_scannerError = false;

// read from file a Pym program
// and return a linked list of tokens retrieved
std::list<Token> scanner(const char* fileName)
{
	g_scannerError = false;
	const std::string fileContent = pym_utils::readFromFile(fileName);
	const char* str = fileContent.c_str();
	std::list<Token> tokenList;
	initDfa();
	while (true)
	{
		Token tmp = dfa(str);
		if (tmp.type == TokenType::DEDENT)
		{
			for (size_t i = 0; i < tmp.str[0]; i++)
			{
				tokenList.emplace_back(TokenType::DEDENT, ""s, tmp.line);
			}
			continue;
		}
		if (tmp.type == TokenType::FEOF)
		{
			for (size_t i = 0; i < tmp.str[0]; i++)
			{
				tokenList.emplace_back(TokenType::DEDENT, ""s, tmp.line);
			}
			tokenList.push_back(std::move(tmp));
			break;
		}
		if (tmp.type == TokenType::ERROR)
		{
			// Report error message immediately.
			fprintf(stderr, "%s:%d:\x001b[31mERROR\x001b[0m: %s\n\n", fileName, tmp.line, tmp.str.c_str());
			g_scannerError = true;
		}
		tokenList.push_back(std::move(tmp));

	}
	return tokenList;
}
