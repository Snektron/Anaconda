#include "parser/token.h"

const std::vector<const char*> Token::types =
{
    "<eoi>", "<unknown>", "<whitespace>", "<newline>",
    "<ident>",
    "(", ")", "{", "}",
    "->", ",", "=", "+", "-", "*", "/", "%", ";",
    "<comment>"
};

const std::vector<const char*> Token::keywords =
{
    "if", "else", "while", "type", "func", "return"
};

const std::vector<const char*> Token::builtinDataTypes =
{
    "u8", "void"
};

Token::Token(const TokenType type):
    span{0, 0}, type(type) {}

Token::Token(const TokenType type, const std::string& text):
    span{0, 0}, type(type), data(text) {}

bool Token::isKeyword() const
{
    if (!isType<TokenType::IDENT>())
        return false;

    std::string text = asText();
    for (auto kw : keywords)
        if (text == kw)
            return true;

    return false;
}

bool Token::isBuiltinDataType() const
{
    if (!isType<TokenType::IDENT>())
        return false;

    std::string text = asText();

    for (auto bdt : builtinDataTypes)
        if (text == bdt)
            return true;

    return false;
}

bool Token::isDataType() const
{
    return isType<TokenType::IDENT>() && !isKeyword();
}

bool Token::isReserved() const
{
    return isKeyword() || isBuiltinDataType();
}

std::string Token::asText() const
{
    return isType<TokenType::IDENT>() ? std::get<std::string>(this->data) : "";
}

DataTypeBase* Token::asDataType() const
{
    if (!isType<TokenType::IDENT>())
        return nullptr;

    std::string text = asText();
    if (text == builtinDataTypes[(std::size_t) BuiltinDataType::U8])
        return new DataType<DataTypeClass::U8>();
    else if (text == builtinDataTypes[(std::size_t) BuiltinDataType::VOID])
        return new DataType<DataTypeClass::VOID>();
    return new DataType<DataTypeClass::STRUCT_FORWARD>(text);
}

bool Token::hasText() const
{
    return isOneOf<TokenType::WHITESPACE, TokenType::IDENT, TokenType::COMMENT>();
}

std::ostream& operator<<(std::ostream& os, const TokenType type)
{
    os << Token::types[(std::size_t) type];
    return os;
}

std::ostream& operator<<(std::ostream& os, const Keyword kw)
{
    os << '\'' << Token::keywords[(std::size_t) kw] << '\'';
    return os;
}

std::ostream& operator<<(std::ostream& os, const Token& token)
{
    if (token.hasText())
    {
        std::string str = token.asText();
        os << '\'' << str << '\'';
    }
    else
        os << token.type;

    return os;
}
