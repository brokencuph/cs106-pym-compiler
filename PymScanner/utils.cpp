#include "utils.h"

//#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

namespace pym_utils
{
	std::string readFromFile(const char* fileName)
	{
		std::ifstream fileStream(fileName, std::ios::binary);
		std::stringstream sStream;
		sStream << fileStream.rdbuf(); // read in the whole file into one string
		return sStream.str();
	}


	bool pymIsSpace(char ch)
	{
		if (isspace(ch) && ch != '\n')
		{
			return true;
		}
		return false;
	}
}
