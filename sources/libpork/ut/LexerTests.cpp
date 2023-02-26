#include <catch2/catch_test_macros.hpp>
#include <Lexer.hpp>
#include <Input.hpp>


TEST_CASE("Lexer basic tests", "[Lexer]")
{
    Lexer lexer;

    SECTION("Keywords scanning")
    {
        StringInput input{"if"};
        Token token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::IF);
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 2u);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 2u);
        REQUIRE(input.isEnded());


        input = StringInput{"else"};
        token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::ELSE);
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 4u);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 4u);
        REQUIRE(input.isEnded());


        input = StringInput{"while"};
        token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::WHILE);
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 5u);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 5u);
        REQUIRE(input.isEnded());


        input = StringInput{"do"};
        token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::DO);
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 2u);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 2u);
        REQUIRE(input.isEnded());
    }


    SECTION("Identifiers scanning")
    {
        StringInput input{"TheName"};
        Token token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::IDENTIFIER);
        REQUIRE(std::get<std::string>(token.value) == "TheName");
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 7u);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 7u);
        REQUIRE(input.isEnded());


        input = StringInput{"_the_name"};
        token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::IDENTIFIER);
        REQUIRE(std::get<std::string>(token.value) == "_the_name");
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 9u);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 9u);
        REQUIRE(input.isEnded());


        input = StringInput{"name1"};
        token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::IDENTIFIER);
        REQUIRE(std::get<std::string>(token.value) == "name1");
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 5u);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 5u);
        REQUIRE(input.isEnded());
    }


    SECTION("Int32 literals scanning")
    {
        StringInput input{"123"};
        Token token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::INT32_LITERAL);
        REQUIRE(std::get<std::int32_t>(token.value) == 123);
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 3u);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 3u);
        REQUIRE(input.isEnded());


        input = StringInput{"123i"};
        token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::INT32_LITERAL);
        REQUIRE(std::get<std::int32_t>(token.value) == 123);
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 4u);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 4u);
        REQUIRE(input.isEnded());


        input = StringInput{"123I"};
        token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::INT64_LITERAL);
        REQUIRE(std::get<std::int64_t>(token.value) == 123);
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 4u);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 4u);
        REQUIRE(input.isEnded());
    }

    SECTION("Symbols scanning")
    {
        StringInput input{"{}()[]<<<>>>+-*/%===;:,.!&&&|||^~"};

        Token token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::L_BRACE);
        REQUIRE(input.currLine() == 1u);
        REQUIRE(input.currCol() == 1u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::R_BRACE);
        REQUIRE(input.currCol() == 2u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::L_PAREN);
        REQUIRE(input.currCol() == 3u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::R_PAREN);
        REQUIRE(input.currCol() == 4u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::L_BRACKET);
        REQUIRE(input.currCol() == 5u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::R_BRACKET);
        REQUIRE(input.currCol() == 6u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::DOUBLE_LESS);
        REQUIRE(input.currCol() == 8u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::LESS);
        REQUIRE(input.currCol() == 9u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::DOUBLE_GR);
        REQUIRE(input.currCol() == 11u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::GREATER);
        REQUIRE(input.currCol() == 12u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::PLUS);
        REQUIRE(input.currCol() == 13u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::MINUS);
        REQUIRE(input.currCol() == 14u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::ASTERISK);
        REQUIRE(input.currCol() == 15u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::SLASH);
        REQUIRE(input.currCol() == 16u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::PERCENT);
        REQUIRE(input.currCol() == 17u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::DOUBLE_EQ);
        REQUIRE(input.currCol() == 19u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::EQUALS);
        REQUIRE(input.currCol() == 20u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::SEMICOLON);
        REQUIRE(input.currCol() == 21u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::COLON);
        REQUIRE(input.currCol() == 22u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::COMMA);
        REQUIRE(input.currCol() == 23u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::DOT);
        REQUIRE(input.currCol() == 24u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::EXCLAMATION);
        REQUIRE(input.currCol() == 25u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::DOUBLE_AMP);
        REQUIRE(input.currCol() == 27u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::AMPERSAND);
        REQUIRE(input.currCol() == 28u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::DOUBLE_PIPE);
        REQUIRE(input.currCol() == 30u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::PIPE);
        REQUIRE(input.currCol() == 31u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::CARET);
        REQUIRE(input.currCol() == 32u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::TILDA);
        REQUIRE(input.currCol() == 33u);

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currCol() == 33u);
    }

    SECTION("Comment skipping while scanning")
    {
        StringInput input{"\
# comment 1\n\
var myVar : Int32 {228i}; # comment 2\n\
# comment 3\n\
        "};
        Lexer lexer;
        Token token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::VAR);
        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::IDENTIFIER);
        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::COLON);
        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::IDENTIFIER);
        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::L_BRACE);
        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::INT32_LITERAL);
        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::R_BRACE);
        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::SEMICOLON);
        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
    }
}
