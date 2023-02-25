#pragma once

#include <string>
#include <string_view>

class Input
{
public:
    static const char8_t END {0x04};

    Input() = default;
    virtual ~Input() = default;

    virtual bool isEnded() const = 0;
    virtual char8_t currChar() const = 0;
    virtual char8_t nextChar() = 0;
    // virtual char lookupChar(std::int32_t offset = 0);

    virtual std::size_t currLine() const = 0;
    virtual std::size_t currCol() const = 0;
};


class StringInput
: public Input
{
public:
    // StringInput() = delete;
    StringInput(std::string && str);
    // StringInput(std::string_view str);
    virtual ~StringInput() = default;

    inline virtual bool isEnded() const override final { return (mCursor == mString.length()); }
    virtual char8_t currChar() const override final;
    virtual char8_t nextChar() override final;

    inline virtual std::size_t currLine() const override final { return mLine; }
    inline virtual std::size_t currCol() const override final { return mCol; }

private:
    std::string mString;
    std::size_t mCursor;
    std::size_t mLine;
    std::size_t mCol;
};
