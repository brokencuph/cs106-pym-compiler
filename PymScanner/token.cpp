#include <utility>
#include "token.h"

Token::Token(TokenType _type, std::string _str, int _line) : type(_type), str(std::move(_str)), line(_line)
{

}