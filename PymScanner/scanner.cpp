#include "scanner.h"
#include "utils.h"

using namespace std::string_literals;

// read from file a Pym program
// and return a linked list of tokens retrieved
std::list<Token> scanner(const char* fileName)
{
	const std::string fileContent = pym_utils::readFromFile(fileName);
	const char* str = fileContent.c_str();
	size_t pos = 0u;
	std::list<Token> tokenList;
	while (str[pos] != '\0')
	{
		Token tmp = dfa(str, &pos);
		tokenList.push_back(tmp);
	}
	tokenList.emplace_back(TokenType::FEOF, ""s);
	return tokenList;
}
