#pragma once

#include "Token.hpp"
#include <string_view>
#include <map>

class Input;

class Lexer
{
public:
    Lexer() = default;
    ~Lexer() = default;

    using TokenTypeMap = std::map<std::string, Token::Type>;

    Token nextToken(Input & input) const;

private:
    Token formErrorToken(std::string_view message, Input & input) const;

    void skipWhitespace(Input & input) const;

    Token scanKeywordOrIdentifier(Input & input) const;
    Token scanNumber(Input & input) const;
    Token scanIntNumber(Input & input, std::string && starting = "") const;
    Token scanRealNumber(Input & input, std::string && starting = "") const;

private:
     static const TokenTypeMap sKeywordMap;
};
