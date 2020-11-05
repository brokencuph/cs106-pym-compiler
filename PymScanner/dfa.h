#ifndef DFA_H_
#define DFA_H_

#include "token.h"

enum class State
{
	START, START_NEWLINE, DONE, NOT_DONE, ERROR,
	IN_NUM_1, IN_NUM_2, // before and after '\'
	IN_ID,
	IN_STR,
	READ_GT, READ_LT, READ_EQ, READ_NEQ,
	READ_MINUS,
	IN_COMMENT,
	IN_INDENT,
};



void initDfa();

Token dfa(const char* str);

#endif