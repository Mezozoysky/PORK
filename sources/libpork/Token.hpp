#pragma once

#include <stdint.h>
#include <string>
#include <string_view>
#include <variant>


struct Token
{
    enum class Type
    {
        ERROR,
        INT32_LITERAL,
        INT64_LITERAL,
        REAL32_LITERAL,
        REAL64_LITERAL,
        STR_LITERAL,
        IDENTIFIER,
        L_BRACE,
        R_BRACE,
        L_PAREN,
        R_PAREN,
        L_BRACKET,
        R_BRACKET,
        PLUS,
        MINUS,
        MULTIPLY,
        DIVIDE,
        LESS,
        GREATER,
        ASSIGN,
        EQUALS,
        SEMICOLON,
        COLON,
        COMMA,
        DOT,
        DOUBLE_DOT,
        MODULE,
        USE,
        FUNC,
        VAR,
        AS,
        IF,
        ELSE,
        WHILE,
        DO,

        END_OF_INPUT
    };

    using ValueType = std::variant<std::int32_t, std::int64_t, float, double, std::string>;

    Type type;
    ValueType value;

    Token(Type aType, ValueType && aValue = 0);
    ~Token() = default;
};
