#include <iostream>
#include "scanner.h"
#include "utils.h"
#include <algorithm>
#include <iterator>

int main()
{
	auto res = scanner("a.pym");
	std::copy(res.begin(), res.end(), std::ostream_iterator<Token>(std::cout, ""));
	return 0;
}