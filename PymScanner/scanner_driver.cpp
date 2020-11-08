#include <iostream>
#include "scanner.h"
#include "utils.h"
#include <algorithm>
#include <iterator>
#include <string>

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		auto res = scanner(argv[1]);
		std::copy(res.begin(), res.end(), std::ostream_iterator<Token>(std::cout, ""));
		return 0;
	}
	std::string fileName;
	std::cout << ":) Hello, what is the name of the Pym source file?" << std::endl;
	std::cout << ">>>";
	std::cin >> fileName;
	auto res = scanner(fileName.c_str());
	std::cout << ":) The listed of tokens are printed as follows:" << std::endl;
	std::cout << std::endl;
	std::copy(res.begin(), res.end(), std::ostream_iterator<Token>(std::cout, ""));

	return 0;
}