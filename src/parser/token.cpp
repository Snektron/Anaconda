#include "parser/token.h"

const std::vector<const char*> Token::keywords =
{
    "if", "else", "while", "type", "func", "return"
};

const std::vector<const char*> Token::builtinDataTypes =
{
    "u8", "void"
};

Token::Token(const TokenType type):
    type(type) {}

Token::Token(const TokenType type, const std::string& text):
    type(type), data(text) {}

bool Token::isKeyword() const
{
    if (!isType<TokenType::IDENT>())
        return false;

    std::string text = asText();
    for (auto kw : keywords)
        if (text == *kw)
            return true;

    return false;
}

bool Token::isBuiltinDataType() const
{
    if (!isType<TokenType::IDENT>())
        return false;

    std::string text = asText();

    for (auto bdt : builtinDataTypes)
        if (text == *bdt)
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
    if (text == builtinDataTypes[BuiltinDataType::U8])
        return new DataType<DataTypeClass::U8>();
    else if (text == builtinDataTypes[BuiltinDataType::VOID])
        return new DataType<DataTypeClass::VOID>();
    return new DataType<DataTypeClass::STRUCT_FORWARD>(text);
}

bool Token::hasText() const
{
    return isOneOf<TokenType::WHITESPACE, TokenType::IDENT, TokenType::COMMENT>();
}
