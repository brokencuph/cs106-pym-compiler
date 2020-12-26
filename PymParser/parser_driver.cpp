#include <iostream>
#include <filesystem>

#include "scanner.h"
#include "parser.h"

using namespace std::string_literals;

void run_scanner_and_parser(const char* fileName)
{
	if (!std::filesystem::exists(fileName))
	{
		std::cout << "File does not exist!\n";
		exit(EXIT_FAILURE);
	}
	auto x = scanner(fileName);
	if (g_scannerError)
	{
		std::cout << "Scanner fails!\n";
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "Scanner succeeds!\n\n";
	}

	// processor between pure scanner and parser:
	// - add NEWLINE before FEOF
	// - remove continuous duplicate NEWLINE's
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
}

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		run_scanner_and_parser(argv[1]);
		return 0;
	}
	std::string fileName;
	std::cout << ":) Hello, what is the name of the Pym source file?" << std::endl;
	std::cout << ">>> ";
	std::getline(std::cin, fileName);
	run_scanner_and_parser(fileName.c_str());
	return 0;
}