
#include <string>
#include "dfa.h"


using namespace std::string_literals;

static State currentState = State::START;

static void (*transfers[13])(char);

static size_t currentPos;

static Token currentToken;

static int currentLine;

void initDfa()
{
	currentState = State::START;
	currentPos = 0;
	currentLine = 1;

	// create function table
	transfers[(int)State::START] = transferStart;
	transfers[(int)State::START_NEWLINE] = transferStartNewline;
	transfers[(int)State::DONE] = transferDone;
	transfers[(int)State::NOT_DONE] = transferNotDone;
	transfers[(int)State::IN_NUM_1] = transferNum1;
	transfers[(int)State::IN_NUM_2] = transferNum2;
	transfers[(int)State::IN_ID] = transferId;
	transfers[(int)State::IN_STR] = transferStr;
	transfers[(int)State::READ_GT] = transferGt;
	transfers[(int)State::READ_LT] = transferLt;
	transfers[(int)State::READ_EQ] = transferEq;
	transfers[(int)State::READ_NEQ] = transferNeq;
	transfers[(int)State::READ_MINUS] = transferMinus;
	transfers[(int)State::IN_COMMENT] = transferComment;
}

static void transferStart(char ch)
{

}

static void transferStartNewline(char ch)
{

}

static void transferDone(char ch)
{
	throw std::exception("Done transfered!");
}

static void transferNotDone(char ch)
{
	throw std::exception("NotDone transfered!");
}

static void transferNum1(char ch)
{

}

static void transferNum2(char ch)
{

}

static void transferId(char ch)
{

}

static void transferStr(char ch)
{

}

static void transferGt(char ch)
{

}

static void transferLt(char ch)
{

}

static void transferEq(char ch)
{

}

static void transferNeq(char ch)
{

}

static void transferMinus(char ch)
{

}

static void transferComment(char ch)
{

}

Token dfa(const char* str)
{
	// do state transfer by looking up the function table
	while (true)
	{
		transfers[(int)currentState](str[currentPos]);
		switch (currentState)
		{
		case State::DONE:
			currentPos++;
			return currentToken;
		case State::NOT_DONE:
		case State::ERROR:
			return currentToken;
		default:
			break;
		}
	}
}
