#include "utils.h"

//#include <iostream>
#include <fstream>
#include <sstream>
namespace pym_utils
{
	std::string readFromFile(const char* fileName)
	{
		std::ifstream fileStream(fileName, std::ios::binary);
		std::stringstream sStream;
		sStream << fileStream.rdbuf(); // read in the whole file into one string
		return sStream.str();
	}
}
