#include <iostream>

#include "scanner.h"
#include "parser.h"

using namespace std::string_literals;

int main()
{
	auto x = scanner("pymtests\\sum100.pym");
	auto sec_to_last = std::prev(x.end());
	x.emplace(sec_to_last, TokenType::NEWLINE, ""s, sec_to_last->line);
	for (auto it = x.begin(); it != x.end(); it++)
	{
		auto nit = std::next(it);
		if (nit == x.end())
		{
			break;
		}
		if (it->type == TokenType::NEWLINE && nit->type == TokenType::NEWLINE)
		{
			x.erase(nit, std::next(nit));
			if (it != x.begin())
				it--;
		}
	}
	Parser parser(x);
	try
	{
		auto res = parser.parse();
		std::cout << *res;
	}
	catch (const std::invalid_argument& e)
	{
		auto tok = parser.getCurrentToken();
		std::cout << "Parser error! " << e.what() << std::endl;
		std::cout << "When processing token: " << tok << std::endl;
	}
	return 0;
}