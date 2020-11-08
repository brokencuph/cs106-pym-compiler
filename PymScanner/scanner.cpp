#include "scanner.h"
#include "token.h"
#include "utils.h"
#include "dfa.h"

using namespace std::string_literals;

// read from file a Pym program
// and return a linked list of tokens retrieved
std::list<Token> scanner(const char* fileName)
{
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
		tokenList.push_back(std::move(tmp));

	}
	return tokenList;
}
