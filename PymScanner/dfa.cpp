
#include <string>
#include <cctype>
#include <unordered_map>
#include <stack>
#include "dfa.h"
#include "utils.h"

using namespace std::string_literals;

using std::string;
using std::stack;

static State currentState = State::START;

static void (*transfers[16])(char);

static size_t currentPos;

static Token currentToken(TokenType::ERROR, ""s, 1);

static int currentLine; // line number

static bool isFirstLine = true;

static const std::unordered_map<std::string, TokenType> keywordsMap = { {"if", TokenType::IF},
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
}; // for keyword quick look up

static stack<int> stIndent; // indentation stack

static bool startFromNewLine = true; // record the next starting state of the DFA
	
static char eofDedent() // return char since it is stored in a string
{
	int cnt = 0;
	while (stIndent.top() != 0)
	{
		cnt++;
		stIndent.pop();
	}
	return (char)cnt;
}

static void changeToken(std::string&& str, TokenType tk, State nextState)
{
	currentToken = Token(tk, std::move(str), currentLine);
	currentState = nextState;
}

//
// Below are state transfer functions, each of which handles one state.
//

static void transferStart(char ch)
{
	if (isdigit(ch)) // into NUM
	{
		changeToken(string(1, ch), TokenType::NUMBER, State::IN_NUM_1);
		return;
	}
	if (isalpha(ch) || ch == '_') // into IDENTIFIER
	{
		changeToken(string(1, ch), TokenType::ID, State::IN_ID);
		return;
	}
	if (pym_utils::pymIsSpace(ch))
	{
		return; // ignore space characters (' ', '\t', etc.)
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
	case '%':
		changeToken(""s, TokenType::MOD, State::DONE);
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
		break;
	case '\0':
		changeToken("\0"s, TokenType::FEOF, State::DONE);
		currentToken.str[0] = eofDedent();
		break;
	case '\n':
		changeToken(""s, TokenType::NEWLINE, State::START_NEWLINE);
		currentLine++;
		break;
	case ' ':
	case '\r':
		break;
	default:
		changeToken("Unexpected token '"s + ch + "'."s, TokenType::ERROR, State::ERROR);
		break;
	}
}

static int indentNum = 0;

static void transferStartNewline(char ch)
{
	if (ch == ' ') // record indentation
	{
		changeToken(""s, TokenType::INDENT, State::IN_INDENT);
		indentNum = 1;
		return;
	}
	else if (ch == '#' || ch == '\n' || ch == '\r') // empty line
	{
		changeToken("\0"s, TokenType::DEDENT, State::NOT_DONE);
		return;
	}
	else if (ch == '\t')
	{
		changeToken("Using Tab as indentation is not supported."s, TokenType::ERROR, State::ERROR);
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
				currentToken.str[0]++; // use str[0] as the token count, which will be handled in scanner.cpp
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
	else if (ch == '#' || ch == '\n' || ch == '\r')
	{
		changeToken("\0"s, TokenType::DEDENT, State::NOT_DONE);
	}
	else if (ch == '\t')
	{
		changeToken("Using Tab as indentation is not supported."s, TokenType::ERROR, State::ERROR);
		return;
	}
	else
	{
		if (isFirstLine && indentNum > 0)
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
				changeToken("Indentation error."s, TokenType::ERROR, State::ERROR);
				return;
			}
		}
		else if (indentNum == stIndent.top())
		{
			changeToken("\0"s, TokenType::DEDENT, State::NOT_DONE);
		}
	}
}

static void transferDone(char ch)
{
	// this is actually not possible
	throw std::exception();
}

static void transferNotDone(char ch)
{
	// this is actually not possible
	throw std::exception();
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
		auto it = keywordsMap.find(currentToken.str); // look up for keyword
		if (it == keywordsMap.end()) 
			currentToken.type = TokenType::ID; // not found in keyword
		else
			currentToken.type = it->second; // record the keyword token type
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
		currentToken.str = "Unexpected newline in string literal.";
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
		currentToken.str = "Unexpected '"s + ch + "'."s;
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
		currentState = State::START_NEWLINE;
		currentLine++;
	}
	else if (ch == '\0') {
		currentToken.type = TokenType::FEOF;
		currentToken.str = "\0"s;
		currentState = State::DONE;
		currentToken.str[0] = eofDedent();
	}
	
}

void initDfa()
{
	currentState = State::START;
	currentPos = 0;
	currentLine = 1;
	isFirstLine = true;

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

void setFirstLineFlag()
{
	if ((currentState == State::DONE ||
		currentState == State::NOT_DONE ||
		currentState == State::START_NEWLINE ||
		currentState == State::ERROR) 
		&& currentToken.type != TokenType::NEWLINE
		&& currentToken.type != TokenType::DEDENT)
	{
		isFirstLine = false;
	}
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
		setFirstLineFlag();
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
