
#include <string>
#include <cctype>
#include "dfa.h"


using namespace std::string_literals;

using std::string;

static State currentState = State::START;

static void (*transfers[16])(char);

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

static void changeToken(std::string&& str, TokenType tk, State nextState)
{
	currentToken = Token(tk, std::move(str), currentLine);
	currentState = nextState;
}

static void transferStart(char ch)
{
	if (isdigit(ch))
	{
		changeToken(string(1, ch), TokenType::NUMBER, State::IN_NUM_1);
		return;
	}
	if (isalpha(ch) || ch == '_')
	{
		changeToken(string(1, ch), TokenType::ID, State::IN_ID);
		return;
	}
	switch (ch)
	{
	case '>':
		changeToken(""s, TokenType::GT, State::READ_GT);
		break;
	case '<':
		changeToken(""s, TokenType::LT, State::READ_LT);
		break;
	case '-':
		changeToken(""s, TokenType::MINUS, State::READ_MINUS);
		break;
	case '=':
		changeToken(""s, TokenType::ASSI, State::READ_EQ);
		break;
	case '!':
		changeToken(""s, TokenType::NEQ, State::READ_NEQ);
		break;
	case '#':
		changeToken(""s, TokenType::CHARS, State::IN_COMMENT);
		break;
	case '+':
		changeToken(""s, TokenType::PLUS, State::DONE);
		break;
	case '*':
		changeToken(""s, TokenType::MUL, State::DONE);
		break;
	case '/':
		changeToken(""s, TokenType::DIV, State::DONE);
		break;
	case '(':
		changeToken(""s, TokenType::LPR, State::DONE);
		break;
	case ')':
		changeToken(""s, TokenType::RPR, State::DONE);
		break;
	case '[':
		changeToken(""s, TokenType::LBR, State::DONE);
		break;
	case ']':
		changeToken(""s, TokenType::RBR, State::DONE);
		break;
	case '"':
		changeToken("\""s, TokenType::CHARS, State::IN_STR);
		break;
	case ',':
		changeToken(""s, TokenType::COMMA, State::DONE);
		break;
	case ':':
		changeToken(""s, TokenType::COLON, State::DONE);
	case '\0':
		changeToken(""s, TokenType::FEOF, State::DONE);
		break;
	case '\n':
		changeToken(""s, TokenType::NEWLINE, State::START_NEWLINE);
		break;
	case ' ':
		break;
	default:
		changeToken("Expected a valid token"s, TokenType::ERROR, State::ERROR);
		break;
	}
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
		currentPos++;
		switch (currentState)
		{
		case State::DONE:
			return currentToken;
		case State::NOT_DONE:
			currentPos--;
		case State::ERROR:
			return currentToken;
		case State::START_NEWLINE:
			return currentToken;
		default:
			break;
		}
	}
}
