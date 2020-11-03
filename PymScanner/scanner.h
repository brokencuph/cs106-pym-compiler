#ifndef SCANNER_H_
#define SCANNER_H_

#include <list> // linked list
#include <string>
#include "token.h"

std::list<Token> scanner(const char* fileName);

#endif