#include <utility>
#include "token.h"

Token::Token(TokenType _type, std::string _str) : type(_type), str(std::move(_str))
{

}