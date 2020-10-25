#ifndef SCANNER_H_
#define SCANNER_H_

#include <list> // linked list
#include <string>
#include "token.h"

std::list<Token> scanner(const char* fileName);

Token dfa(const char* str, size_t* start_pos);

#endif