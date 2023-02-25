#include <catch2/catch_test_macros.hpp>
#include <Token.hpp>

TEST_CASE("Token basic tests", "[Token]")
{
    std::string identifier{"myVar"};
    Token token{Token::Type::IDENTIFIER, std::move(identifier)};
    REQUIRE(token.type == Token::Type::IDENTIFIER);
    REQUIRE(std::get<std::string>(token.value) == "myVar");
}
