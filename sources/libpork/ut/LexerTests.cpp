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
        REQUIRE(std::get<std::string>(token.value) == "if");
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 2);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 2);
        REQUIRE(input.isEnded());


        input = StringInput{"else"};
        token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::ELSE);
        REQUIRE(std::get<std::string>(token.value) == "else");
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 4);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 4);
        REQUIRE(input.isEnded());


        input = StringInput{"while"};
        token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::WHILE);
        REQUIRE(std::get<std::string>(token.value) == "while");
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 5);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 5);
        REQUIRE(input.isEnded());


        input = StringInput{"do"};
        token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::DO);
        REQUIRE(std::get<std::string>(token.value) == "do");
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 2);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 2);
        REQUIRE(input.isEnded());
    }


    SECTION("Identifiers scanning")
    {
        StringInput input{"TheName"};
        Token token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::IDENTIFIER);
        REQUIRE(std::get<std::string>(token.value) == "TheName");
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 7);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 7);
        REQUIRE(input.isEnded());


        input = StringInput{"_the_name"};
        token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::IDENTIFIER);
        REQUIRE(std::get<std::string>(token.value) == "_the_name");
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 9);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 9);
        REQUIRE(input.isEnded());


        input = StringInput{"name1"};
        token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::IDENTIFIER);
        REQUIRE(std::get<std::string>(token.value) == "name1");
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 5);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 5);
        REQUIRE(input.isEnded());
    }


    SECTION("Int32 literals scanning")
    {
        StringInput input{"123"};
        Token token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::INT32_LITERAL);
        REQUIRE(std::get<std::int32_t>(token.value) == 123);
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 3);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 3);
        REQUIRE(input.isEnded());


        input = StringInput{"123i"};
        token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::INT32_LITERAL);
        REQUIRE(std::get<std::int32_t>(token.value) == 123);
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 4);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 4);
        REQUIRE(input.isEnded());


        input = StringInput{"123I"};
        token = lexer.nextToken(input);

        REQUIRE(token.type == Token::Type::INT64_LITERAL);
        REQUIRE(std::get<std::int64_t>(token.value) == 123);
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 4);
        REQUIRE(input.isEnded());

        token = lexer.nextToken(input);
        REQUIRE(token.type == Token::Type::END_OF_INPUT);
        REQUIRE(input.currLine() == 1);
        REQUIRE(input.currCol() == 4);
        REQUIRE(input.isEnded());
    }
}
