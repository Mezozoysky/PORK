#include "Lexer.hpp"
#include "Input.hpp"
#include <fmt/format.h>
#include <cctype>
#include <climits>
//#include <cfloat>
#include <string>
#include <cassert>

#if !defined(CHAR_WIDTH)
#if UCHAR_MAX == 0xFFu
#define CHAR_WIDTH 8u
#else
#error Unsupported char width
#endif
#endif //!defined(CHAR_WIDTH)

#if !defined(SHRT_WIDTH)
#if USHRT_MAX == 0xFFu
#define SHRT_WIDTH 8u
#elif USHRT_MAX == 0xFFFFu
#define SHRT_WIDTH 16u
#elif USHRT_MAX == 0xFFFFFFFFu
#define SHRT_WIDTH 32u
#else
#error Unsupported short int width
#endif
#endif //!defined(SHRT_WIDTH)

#if !defined(INT_WIDTH)
#if UINT_MAX == 0xFFFFu
#define INT_WIDTH 16u
#elif UINT_MAX == 0xFFFFFFFFu
#define INT_WIDTH 32u
#elif UINT_MAX == 0xFFFFFFFFFFFFFFFFu
#define INT_WIDTH 64u
#else
#error Unsupported int width
#endif
#endif //!defined(INT_WIDTH)

#if !defined(LONG_WIDTH)
#if ULONG_MAX == 0xFFFFFFFFu
#define LONG_WIDTH 32u
#elif ULONG_MAX == 0xFFFFFFFFFFFFFFFFu
#define LONG_WIDTH 64u
#else Unsupported long int width
#endif
#endif //!defined(LONG_WIDTH)

#if !defined(LLONG_WIDTH)
#if ULLONG_MAX == 0xFFFFFFFFFFFFFFFFu
#define LLONG_WIDTH 64u
#elif ULLONG_MAX == 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFu
#define LLONG_WIDTH 128u
#else
#error Unsupported long long int width
#endif
#endif //!defined(LLONG_WIDTH)

#if INT_WIDTH == 32u
auto const & str2int32 = static_cast<int(*)(std::string const &, std::size_t *, int)>(std::stoi);
#elif LONG_WIDTH == 32u
auto const & str2int32 = static_cast<long(*)(std::string const &, std::size_t *, int)>(std::stol);
#else
#error No from-string convertion method for Int32
#endif

#if LONG_WIDTH == 64u
auto const & str2int64 = static_cast<long(*)(std::string const &, std::size_t *, int)>(std::stol);
#elif LLONG_WIDTH == 64u
auto const & str2int64 = static_cast<long long(*)(std::string const &, std::size_t *, int)>(std::stoll);
#else
#error No from-string conversion method for Int64
#endif

auto const & str2real32 = static_cast<float(*)(std::string const &, std::size_t *)>(std::stof);
auto const & str2real64 = static_cast<double(*)(std::string const &, std::size_t *)>(std::stod);


const Lexer::TokenTypeMap Lexer::sKeywordMap
{
    {"module", Token::Type::MODULE},
    {"use", Token::Type::USE},
    {"func", Token::Type::FUNC},
    {"var", Token::Type::VAR},
    {"as", Token::Type::AS},
    {"if", Token::Type::IF},
    {"else", Token::Type::ELSE},
    {"while", Token::Type::WHILE},
    {"do", Token::Type::DO},
};

//const Lexer::TokenTypeMap Lexer::sSymbolMap
//{
//    {"{", Token::Type::L_BRACE},
//    {"}", Token::Type::R_BRACE},
//    {"(", Token::Type::L_PAREN},
//    {")", Token::Type::R_PAREN},
//    {"[", Token::Type::L_BRACKET},
//    {"]", Token::Type::R_BRACKET},
//    {"<", Token::Type::LESS},
//    {"<<", Token::Type::DOUBLE_LESS},
//    {">", Token::Type::GREATER},
//    {">>", Token::Type::DOUBLE_GR},
//    {"+", Token::Type::PLUS},
//    {"-", Token::Type::MINUS},
//    {"*", Token::Type::ASTERISK},
//    {"/", Token::Type::SLASH},
//    {"%", Token::Type::PERCENT},
//    {"=", Token::Type::EQUALS},
//    {"==", Token::Type::DOUBLE_EQ},
//    {";", Token::Type::SEMICOLON},
//    {":", Token::Type::COLON},
//    {",", Token::Type::COMMA},
//    {".", Token::Type::DOT},
//    {"..", Token::Type::DOUBLE_DOT},
//    {"?", Token::Type::QUESTION},
//    {"!", Token::Type::EXCLAMATION},
//    {"&", Token::Type::AMPERSAND},
//    {"&&", Token::Type::DOUBLE_AMP},
//    {"|", Token::Type::PIPE},
//    {"||", Token::Type::DOUBLE_PIPE},
//    {"^", Token::Type::CARET},
//    {"~", Token::Type::TILDA},
//    {"$", Token::Type::DOLLAR},
//    {"@", Token::Type::AT}
//};

Token Lexer::nextToken(Input & input) const
{
    char ch = input.currChar();
    assert(std::isgraph(ch) || std::isspace(ch) || ch == Input::END);
    if (!std::isgraph(ch) && !std::isspace(ch) && ch != Input::END)
    {
        return { Token::Type::ERROR, fmt::format("unexpected character (code: {}", int(ch)) };
    }

    while (ch != Input::END)
    {
        if (std::isspace(ch))
        {
            // skip whitespace
            ch = input.nextChar();
        }
        else if (ch == '_' || std::isalpha(ch))
        {
            return scanKeywordOrIdentifier(input);
        }
        else if (std::isdigit(ch))
        {
            return scanNumber(input);
        }
        else if (std::ispunct(ch))
        {
            std::string term;
            if (ch == '#')
            {
                // skip rest of line as comment
                do
                {
                    ch = input.nextChar();
                }
                while (ch != '\n' && ch != Input::END);
                if (ch == '\n')
                {
                    input.nextChar();
                }

                continue;
            }
            return scanSymbol(input);
        }
    }

    return {Token::Type::END_OF_INPUT};
}

Token Lexer::formErrorToken(std::string_view message, Input & input) const
{
    return {Token::Type::ERROR, fmt::format("Lexical error near {}, {}: {}\n", input.currLine(),  input.currCol(), message.data())};
}

Token Lexer::scanKeywordOrIdentifier(Input & input) const
{
    char ch = input.currChar();
    assert(ch == '_' || std::isalpha(ch));

    std::string term;
    while (ch == '_' || std::isalpha(ch) || std::isdigit(ch))
    {
        term += ch;
        ch = input.nextChar();
    }

    auto kwIt = sKeywordMap.find(term);
    if (kwIt != sKeywordMap.end())
    {
        return {kwIt->second, std::move(term)};
    }
    return {Token::Type::IDENTIFIER, std::move(term)};
}

Token Lexer::scanNumber(Input & input) const
{
    char ch = input.currChar();
    assert(std::isdigit(ch));

    std::string term;
    while (std::isdigit(ch))
    {
        term += ch;
        ch = input.nextChar();
    }

    if (ch == '.')
    {
        return scanRealNumber(input, std::move(term));
    }

    return scanIntNumber(input, std::move(term));
}

Token Lexer::scanIntNumber(Input & input, std::string && starting) const
{
    std::string term{std::move(starting)};
    int radix{10};
    if (term.starts_with("0x"))
    {
        radix = 16;
    }
    else if (term.starts_with("0o"))
    {
        radix = 8;
    }
    else if (term.starts_with("0b"))
    {
        radix = 2;
    }

    char ch = input.currChar();
    while (std::isdigit(ch))
    {
        term += ch;
        ch = input.nextChar();
    }

    if (ch  ==  '_')
    {
        return formErrorToken("wrong number literal suffix '_'", input);
    }
    else if (std::isalpha(ch))
    {
        if (ch == 'I')
        {
            std::int64_t value;
            try
            {
                value = str2int64(term, nullptr, radix);
            }
            catch (std::invalid_argument const & ex)
            {
                return formErrorToken(fmt::format("'{}i' isn't an Int64 literal", term), input);
            }
            catch (std::out_of_range const & ex)
            {
                return formErrorToken(fmt::format("literal '{}i' can't be represented as Int64", term), input);
            }

            input.nextChar();
            return {Token::Type::INT64_LITERAL, std::move(value)};
        }
        else if (ch == 'i')
        {
            input.nextChar();
        }
        else
        {
            return formErrorToken(fmt::format("wrong number literal suffic '{}'", ch), input);
        }
    }

    std::int32_t value;
    try
    {
        value = str2int32(term, nullptr, radix);
    }
    catch (std::invalid_argument const & ex)
    {
        return formErrorToken(fmt::format("'{}i' isn't an Int32 literal", term), input);
    }
    catch (std::out_of_range const & ex)
    {
        return formErrorToken(fmt::format("literal '{}i' can't be represented as Int32", term), input);
    }

    return {Token::Type::INT32_LITERAL, std::move(value)};
}

Token Lexer::scanRealNumber(Input & input, std::string && starting) const
{
    return formErrorToken("Real numbers are not implemented yet", input);
}

Token Lexer::scanSymbol(Input & input) const
{
    char ch = input.currChar();
    assert(std::ispunct(ch));

    std::string term{ch};
    if (ch == '{')
    {
        input.nextChar();
        return {Token::Type::L_BRACE};
    }
    else if (ch == '}')
    {
        input.nextChar();
        return {Token::Type::R_BRACE};
    }
    else if (ch == '(')
    {
        input.nextChar();
        return {Token::Type::L_PAREN};
    }
    else if (ch == ')')
    {
        input.nextChar();
        return {Token::Type::R_PAREN};
    }
    else if (ch == '[')
    {
        input.nextChar();
        return {Token::Type::L_BRACKET};
    }
    else if (ch == ']')
    {
        input.nextChar();
        return {Token::Type::R_BRACKET};
    }
    else if (ch == '<')
    {
        ch = input.nextChar();
        if (ch == '<')
        {
            input.nextChar();
            return {Token::Type::DOUBLE_LESS};
        }

        return {Token::Type::LESS};
    }
    else if (ch == '>')
    {
        ch = input.nextChar();
        if (ch == '>')
        {
            input.nextChar();
            return {Token::Type::DOUBLE_GR};
        }

        return {Token::Type::GREATER};
    }
    else if (ch == '+')
    {
        input.nextChar();
        return {Token::Type::PLUS};
    }
    else if (ch == '-')
    {
        input.nextChar();
        return {Token::Type::MINUS};
    }
    else if (ch == '*')
    {
        input.nextChar();
        return {Token::Type::ASTERISK};
    }
    else if (ch == '/')
    {
        input.nextChar();
        return {Token::Type::SLASH};
    }
    else if (ch == '%')
    {
        input.nextChar();
        return {Token::Type::PERCENT};
    }
    else if (ch == '=')
    {
        ch = input.nextChar();
        if (ch == '=')
        {
            input.nextChar();
            return {Token::Type::DOUBLE_EQ};
        }

        return {Token::Type::EQUALS};
    }
    else if (ch == ';')
    {
        input.nextChar();
        return {Token::Type::SEMICOLON};
    }
    else if (ch == ':')
    {
        input.nextChar();
        return {Token::Type::COLON};
    }
    else if (ch == ',')
    {
        input.nextChar();
        return {Token::Type::COMMA};
    }
    else if (ch == '.')
    {
        ch = input.nextChar();
//        if (ch == '.')
//        {
//            input.nextChar();
//            return {Token::Type::DOUBLE_DOT};
//        }

        return {Token::Type::DOT};
    }
//    else if (ch == '?')
//    {
//        input.nextChar();
//        return {Token::Type::QUESTION};
//    }
    else if (ch == '!')
    {
        input.nextChar();
        return {Token::Type::EXCLAMATION};
    }
    else if (ch == '&')
    {
        ch = input.nextChar();
        if (ch == '&')
        {
            input.nextChar();
            return {Token::Type::DOUBLE_AMP};
        }

        return {Token::Type::AMPERSAND};
    }
    else if (ch == '|')
    {
        ch = input.nextChar();
        if (ch == '|')
        {
            input.nextChar();
            return {Token::Type::DOUBLE_PIPE};
        }

        return {Token::Type::PIPE};
    }
    else if (ch == '^')
    {
        input.nextChar();
        return {Token::Type::CARET};
    }
    else if (ch == '~')
    {
        input.nextChar();
        return {Token::Type::TILDA};
    }
//    else if (ch == '$')
//    {
//        input.nextChar();
//        return {Token::Type::DOLLAR};
//    }
//    else if (ch == '@')
//    {
//        input.nextChar();
//        return {Token::Type::AT};
//    }

    return formErrorToken(fmt::format("unexpected character '{}' (code: {}) while scanning symbol", ch, int{ch}), input);
}
