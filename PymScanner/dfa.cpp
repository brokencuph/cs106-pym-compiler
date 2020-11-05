
#include <string>
#include <cctype>
#include <unordered_map>
#include <stack>
#include "dfa.h"


using namespace std::string_literals;

using std::string;
using std::stack;

static State currentState = State::START;

static void (*transfers[16])(char);

static size_t currentPos;

static Token currentToken(TokenType::ERROR, ""s, 1);

static int currentLine;

static std::unordered_map<std::string, TokenType> keywordsMap = { {"if", TokenType::IF},
	{"else",TokenType::ELSE},
	{"elif",TokenType::ELIF},
	{"while",TokenType::WHILE},
	{"def",TokenType::DEF},
	{"return",TokenType::RETURN},
	{"and",TokenType::AND},
	{"or",TokenType::OR},
	{"not",TokenType::NOT},
	{"int",TokenType::INT},
	{"num",TokenType::NUM},
	{"str",TokenType::STR}
};

static stack<int> stIndent;

static bool startFromNewLine = true;
	

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
		currentLine++;
		break;
	case ' ':
		break;
	default:
		changeToken("Expected a valid token"s, TokenType::ERROR, State::ERROR);
		break;
	}
}

static int indentNum = 0;

static void transferStartNewline(char ch)
{
	if (ch == ' ')
	{
		changeToken(""s, TokenType::INDENT, State::IN_INDENT);
		indentNum = 1;
		return;
	}
	else
	{
		const int indentNum = 0;
		changeToken(string(1, '\0'), TokenType::DEDENT, State::NOT_DONE);
		if (indentNum < stIndent.top())
		{
			
			while (stIndent.top() > indentNum)
			{
				stIndent.pop();
				currentToken.str[0]++; // use str[0] as the token count
			}
			if (stIndent.top() < indentNum)
			{
				changeToken("Wrong indentation"s, TokenType::ERROR, State::ERROR);
				return;
			}
		}
	}
}

static void transferIndent(char ch)
{
	if (ch == ' ')
	{
		indentNum++;
		return;
	}
	else
	{
		if (currentLine == 1 && indentNum > 0)
		{
			changeToken("The first line is indented."s, TokenType::ERROR, State::ERROR);
			return;
		}
		if (indentNum > stIndent.top())
		{
			stIndent.push(indentNum);
			changeToken(""s, TokenType::INDENT, State::NOT_DONE);
			return;
		}
		else if (indentNum < stIndent.top())
		{
			changeToken("\0"s, TokenType::DEDENT, State::NOT_DONE);
			while (stIndent.top() > indentNum)
			{
				stIndent.pop();
				currentToken.str[0]++; // use str[0] as the token count
			}
			if (stIndent.top() < indentNum)
			{
				changeToken("Wrong indentation"s, TokenType::ERROR, State::ERROR);
				return;
			}
		}
	}
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
	if (isdigit(ch))
	{
		currentToken.str.append(1, ch);
		return;
	}
	else if (ch == '\\')
	{
		currentState = State::IN_NUM_2;
		currentToken.str.append(1, ch);
	}
	else
		currentState = State::NOT_DONE;
}

static void transferNum2(char ch)
{
	if (isdigit(ch))
	{
		currentToken.str.append(1, ch);
		return;
	}
	else
		currentState = State::NOT_DONE;
}

static void transferId(char ch)
{
	if (isalnum(ch) || ch=='_') 
	{
		currentToken.str.append(1, ch);
		auto it = keywordsMap.find(currentToken.str);
		if (it == keywordsMap.end())
			currentToken.type = TokenType::ID;
		else
			currentToken.type = it->second;
		return;
	}
	else {
		if (currentToken.type != TokenType::ID)
			currentToken.str.clear();
		currentState = State::NOT_DONE;
	}
}

static void transferStr(char ch)
{
	if (ch == '"') {
		currentToken.str.append(1, ch);
		currentState = State::DONE;
	}
	else if (ch == '\n') {
		currentToken.type = TokenType::ERROR;
		currentState = State::ERROR;
		currentToken.str = "Unexpected newline";
	}
	else {
		currentToken.str.append(1, ch);
	}
}

static void transferGt(char ch)
{
	if (ch == '=') {
		currentToken.type = TokenType::GE;
		currentState = State::DONE;
	}
	else {
		currentState = State::NOT_DONE;
	}

}

static void transferLt(char ch)
{
	if (ch == '=') {
		currentToken.type = TokenType::LE;
		currentState = State::DONE;
	}
	else {
		currentState = State::NOT_DONE;
	}
}

static void transferEq(char ch)
{
	if (ch == '=') {
		currentToken.type = TokenType::EQ;
		currentState = State::DONE;
	}
	else {
		currentState = State::NOT_DONE;
	}
}

static void transferNeq(char ch)
{
	if (ch == '=') {
		currentToken.type = TokenType::NEQ;
		currentState = State::DONE;
	}
	else {
		currentToken.type = TokenType::ERROR;
		currentToken.str = "unexpected '"s + ch + "'"s;
		currentState = State::ERROR;
	}
}

static void transferMinus(char ch)
{

	if (ch == '>') {
		currentToken.type = TokenType::ARROW;
		currentState = State::DONE;
	}
	else {
		currentState = State::NOT_DONE;
	}
}

static void transferComment(char ch)
{
	if (ch == '\n') {
		currentToken.type = TokenType::NEWLINE;
		currentState = State::DONE;
	}
	else if (ch == '\0') {
		currentToken.type = TokenType::FEOF;
		currentState = State::DONE;
	}
	
}

void initDfa()
{
	currentState = State::START;
	currentPos = 0;
	currentLine = 1;

	startFromNewLine = true;

	stIndent.push(0);

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
	transfers[(int)State::IN_INDENT] = transferIndent;
}


Token dfa(const char* str)
{
	if (startFromNewLine)
	{
		currentState = State::START_NEWLINE;
		startFromNewLine = false;
	}
	else
	{
		currentState = State::START;
	}
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
			startFromNewLine = true;
			return currentToken;
		default:
			break;
		}
	}
}
