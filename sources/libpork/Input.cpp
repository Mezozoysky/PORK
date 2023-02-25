#include "Input.hpp"
#include <cassert>


StringInput::StringInput(std::string && str)
: mString {std::move(str)}
, mCursor {0u}
, mLine {1u}
, mCol {0u}
{
}

// StringInput::StringInput(std::stirng_view str)
// : mString {str}
// , mCursor {0u}
// , mLine {0u}
// , mCol {0u}
// {
// }

char8_t StringInput::currChar() const
{
    if (isEnded())
    {
        return END;
    }
    return mString.data()[mCursor];
}

char8_t StringInput::nextChar()
{
    assert(!isEnded() && "nextChar() method calling is forbidden after end of input is reached");
    if (!isEnded())
    {
        char8_t ch = mString.data()[mCursor];
        if (ch == '\n')
        {
            ++mLine;
            mCol = 0;
        }
        else if (ch == '\r')
        {
            // just skip it
        }
        else
        {
            ++mCol;
        }
        ++mCursor;
    }

    return currChar();
}
