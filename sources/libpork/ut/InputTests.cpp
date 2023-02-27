#include <catch2/catch_test_macros.hpp>
#include <Input.hpp>


TEST_CASE("StringInput basic tests", "[Input,StringInput]")
{
    std::string source = "\
module My;\n\
\n\
const PI : Real32 {3.1415926};\n\
\n\
func squareReal(x : Real32) -> Real32\n\
{\n\
    return (x * x);\n\
}\n\
\n\
func main() -> Int32\n\
{\n\
use Io.print;\n\
\n\
const radius : Int32 = 10;\n\
var area : Real32 {PI * squareReal(radius as Real32)};\n\
print(\"Area of circle with radius == {}: {}\", radius as String, area as String);\n\
\n\
return 0;\n\
";

    StringInput input{std::move(source)};
    REQUIRE(!input.isEnded());
    REQUIRE(input.currLine() == 1u);
    REQUIRE(input.currCol() == 0u);
    REQUIRE(input.currChar() == 'm');

    while (!input.isEnded())
    {
        input.nextChar();
    }
    REQUIRE(input.currChar() == Input::END);
    REQUIRE(input.currLine() == 23u);
}
