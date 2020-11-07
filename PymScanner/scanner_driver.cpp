#include <iostream>
#include "scanner.h"
#include "utils.h"
#include <algorithm>
#include <iterator>

int main()
{
	char* fileName;
	fileName = new char[10000];
	std::cout << ":) Hello, what is the name of the Pym source file?" << std::endl;
	std::cout << ">>>";
	std::cin >> fileName;
	auto res = scanner(fileName);
	std::cout << ":) The listed of tokens are printed as follows:" << std::endl;
	std::cout << std::endl;
	std::copy(res.begin(), res.end(), std::ostream_iterator<Token>(std::cout, ""));

	delete[] fileName;
	return 0;
}