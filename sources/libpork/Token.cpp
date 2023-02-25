#include "Token.hpp"

Token::Token(Type aType, ValueType && aValue)
: type{aType}
, value{std::move(aValue)}
{
}
