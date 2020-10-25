#include <iostream>
#include "utils.h"

int main()
{
	std::cout << pym_utils::readFromFile("utils.cpp") << std::endl;

	return 0;
}