#ifndef UTILS_H_
#define UTILS_H_

#include <string>

namespace pym_utils
{
	/// <summary>
	/// Read file into std::string.
	/// </summary>
	/// <param name="fileName">The file to read</param>
	/// <returns>A string containing the file content</returns>
	std::string readFromFile(const char* fileName);

	/// <summary>
	/// Check whether a character is a Pym space character (all common space characters except newline)
	/// </summary>
	/// <param name="ch">The character to be checked.</param>
	/// <returns></returns>
	bool pymIsSpace(char ch);
}

#endif