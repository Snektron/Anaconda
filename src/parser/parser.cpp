#include "parser/parser.h"
#include "common/format.h"
#include <sstream>

#define TRACE_ENABLE

#ifdef TRACE_ENABLE
#include <iostream>

struct Trace
{
    const char* func;

    Trace(const char* func):
        func(func)
    {
        std::cout << "Entering " << func << std::endl;
    }

    ~Trace()
    {
        std::cout << "Exiting " << func << std::endl;
    }
};

#define TRACE Trace _trace_##__FUNCTION__(__FUNCTION__)
#else
#define TRACE
#endif

SyntaxError::SyntaxError(const Span& span, const std::string& msg):
    std::runtime_error(fmt::sprintf("Error at ", span, ": ", msg, "."))
{}

Parser::Parser(std::istream& input):
    lexer(input), token(Span{0, 0}, TokenType::EOI)
{
    consume();
}

[[noreturn]]
void Parser::error(const std::string& msg)
{
    throw SyntaxError(this->token.span, msg);
}

std::unique_ptr<GlobalNode> Parser::program()
{
    TRACE;
    return this->prog();
}

const Token& Parser::consume()
{
    do
       this->token = this->lexer.next();
    while (this->token.isOneOf<TokenType::WHITESPACE, TokenType::COMMENT, TokenType::NEWLINE>());

    std::cout << "tok: " << this->token << std::endl;

    return this->token;
}

// <unit> = <globalstat>*
std::unique_ptr<GlobalNode> Parser::prog()
{
    TRACE;
    std::vector<std::unique_ptr<GlobalElementNode>> elements;

    while (!this->check<TokenType::EOI>())
        elements.push_back(globalstat());

    std::vector<GlobalElementNode*> tmp;

    for (auto& elem : elements)
        tmp.push_back(elem.release());
    return std::make_unique<GlobalNode>(tmp);
}

// <globalstat> = <funcdecl> | <structdecl> | <globalexpr>
std::unique_ptr<GlobalElementNode> Parser::globalstat()
{
    TRACE;
    if (this->check<TokenType::FUNC>())
        return this->funcdecl();

    if (this->check<TokenType::TYPE>())
        return this->structdecl();

    return this->globalexpr();
}

std::unique_ptr<GlobalExpressionNode> Parser::globalexpr()
{
    TRACE;
    auto expr = this->expr();
    this->expect<TokenType::SEMICOLON>();
    return std::make_unique<GlobalExpressionNode>(expr.release());
}

// <structdecl> = 'type' <id> '{' <fieldlist> '}'
std::unique_ptr<StructureDefinitionNode> Parser::structdecl()
{
    TRACE;
    this->expect<TokenType::TYPE>();

    const std::string name = this->ident();

    this->expect<TokenType::BRACE_OPEN>();

    if (this->eat<TokenType::BRACE_CLOSE>())
        return std::make_unique<StructureDefinitionNode>(name, new FieldListNode(std::vector<Field>()));

    auto members = fieldlist();

    this->expect<TokenType::BRACE_CLOSE>();

    return std::make_unique<StructureDefinitionNode>(name, members.release());
}

// <funcdecl> = 'func' <id> <funcpar> ('->' <id>)? <block>
std::unique_ptr<FunctionDeclaration> Parser::funcdecl()
{
    TRACE;
    this->expect<TokenType::FUNC>();
    const std::string name = this->ident();

    auto parameters = funcpar();

    std::unique_ptr<DataTypeBase> rtype(nullptr);

    if (this->eat<TokenType::ARROW>())
        rtype = this->datatype();
    else
        rtype = std::make_unique<DataType<DataTypeClass::VOID>>();

    auto body = block();

    return std::make_unique<FunctionDeclaration>(name, parameters.release(), rtype.release(), body.release());
}

// <funcpar> = '(' <fieldlist>? ')'
std::unique_ptr<FieldListNode> Parser::funcpar()
{
    TRACE;
    this->expect<TokenType::PAREN_OPEN>();

    if (this->eat<TokenType::PAREN_CLOSE>())
        return std::make_unique<FieldListNode>(std::vector<Field>());

    auto parameters = fieldlist();
    this->expect<TokenType::PAREN_CLOSE>();
    return parameters;
}

// <fieldlist> = <type> <id> (',' <type>? <id>)*
std::unique_ptr<FieldListNode> Parser::fieldlist()
{
    TRACE;
    std::vector<Field> parameters;

    auto lasttype = datatype();
    const std::string name = this->ident();

    parameters.push_back(Field(lasttype->copy(), name));

    while (true)
    {
        if (!this->eat<TokenType::COMMA>())
            break;

        Token saved = this->token;
        if (!this->token.isDataType()) // also includes isType<IDENT>
            this->expected("datatype or identifier");
        this->consume();

        if (this->check<TokenType::IDENT>())
        {
            std::string name = this->ident();
            lasttype = std::unique_ptr<DataTypeBase>(saved.asDataType());
            parameters.push_back(Field(lasttype->copy(), name));
        }
        else
        {
            std::string name = saved.lexeme.get<std::string>();
            parameters.push_back(Field(lasttype->copy(), name));
        }
    }

    return std::make_unique<FieldListNode>(parameters);
}

// <block> = '{' <statement>* '}'
std::unique_ptr<BlockNode> Parser::block()
{
    TRACE;
    std::unique_ptr<StatementNode> list = std::make_unique<EmptyStatementNode>();
    this->expect<TokenType::BRACE_OPEN>();

    while (!this->eat<TokenType::BRACE_CLOSE>())
    { 
        auto node = statement();
        list = std::make_unique<StatementListNode>(list.release(), node.release());
    }

    return std::make_unique<BlockNode>(list.release());
}

// <statement> = <ifstat> | <whilestat>
std::unique_ptr<StatementNode> Parser::statement()
{
    TRACE;
    if (this->check<TokenType::IF>())
        return ifstat();

    if (this->check<TokenType::WHILE>())
        return whilestat();

    if (this->check<TokenType::RETURN>())
        return returnstat();

    if (this->check<TokenType::BRACE_OPEN>())
        return block();

    return this->exprstat();
}

std::unique_ptr<ReturnNode> Parser::returnstat()
{
    TRACE;
    this->expect<TokenType::RETURN>();
    auto expr = this->expr();
    return std::make_unique<ReturnNode>(expr.release());
}

std::unique_ptr<StatementNode> Parser::exprstat()
{
    TRACE;
    auto expr = this->expr();

    if (this->eat<TokenType::SEMICOLON>())
        return std::make_unique<ExpressionStatementNode>(expr.release());
    return std::make_unique<ReturnNode>(expr.release());
}

// <ifstat> = 'if' <expr> <block> ('else' (<ifstat> | <block>))?
std::unique_ptr<StatementNode> Parser::ifstat()
{
    TRACE;
    this->expect<TokenType::IF>();

    auto condition = expr();
    auto consequent = statement();

    if (this->eat<TokenType::ELSE>())
    {
        auto alternative = statement();
        return std::make_unique<IfElseNode>(condition.release(), consequent.release(), alternative.release());
    }

    return std::make_unique<IfNode>(condition.release(), consequent.release());
}

// <whilestat> = 'while' <expr> <block>
std::unique_ptr<WhileNode> Parser::whilestat()
{
    TRACE;
    this->expect<TokenType::WHILE>();

    auto condition = expr();
    auto consequent = statement();

    return std::make_unique<WhileNode>(condition.release(), consequent.release());
}

// <expr> = <sum>
std::unique_ptr<ExpressionNode> Parser::expr()
{
    TRACE;
    return this->bor();
}

std::unique_ptr<ExpressionNode> Parser::bor()
{
    TRACE;
    auto lhs = this->bxor();

    while (true)
    {
        if (!this->eat<TokenType::PIPE>())
            break;

        auto rhs = this->bxor();
        lhs = std::make_unique<BitwiseOrNode>(lhs.release(), rhs.release());
    }

    return lhs;
}

std::unique_ptr<ExpressionNode> Parser::bxor()
{
    TRACE;
    auto lhs = this->band();

    while (true)
    {
        if (!this->eat<TokenType::HAT>())
            break;

        auto rhs = this->band();
        lhs = std::make_unique<BitwiseXorNode>(lhs.release(), rhs.release());
    }

    return lhs;
}

std::unique_ptr<ExpressionNode> Parser::band()
{
    TRACE;
    auto lhs = this->shift();

    while (true)
    {
        if (!this->eat<TokenType::AMPERSAND>())
            break;

        auto rhs = this->shift();
        lhs = std::make_unique<BitwiseAndNode>(lhs.release(), rhs.release());
    }

    return lhs;
}

std::unique_ptr<ExpressionNode> Parser::shift()
{
    TRACE;
    auto lhs = this->sum();

    while (true)
    {
        TokenType optype = this->token.type;
        if (!this->eatOneOf<TokenType::LEFTLEFT, TokenType::RIGHTRIGHT>())
            break;

        auto rhs = this->sum();
        lhs = this->toBinOp(optype, std::move(lhs), std::move(rhs));
    }

    return lhs;
}

// <sum> = <product> (('+' | '-') <product>)*
std::unique_ptr<ExpressionNode> Parser::sum()
{
    TRACE;
    auto lhs = this->product();

    while (true)
    {
        TokenType optype = this->token.type;
        if (!this->eatOneOf<TokenType::PLUS, TokenType::MINUS>())
            break;

        auto rhs = this->product();
        lhs = this->toBinOp(optype, std::move(lhs), std::move(rhs));
    }

    return lhs;
}

// <product> = <unary> (('*' | '/' | '%') <unary>)*
std::unique_ptr<ExpressionNode> Parser::product()
{
    TRACE;
    auto lhs = this->unary();

    while (true)
    {
        TokenType optype = this->token.type;
        if (!this->eatOneOf<TokenType::STAR, TokenType::SLASH, TokenType::PERCENT>())
            break;

        auto rhs = this->unary();
        lhs = this->toBinOp(optype, std::move(lhs), std::move(rhs));
    }

    return lhs;
}

// <unary> = '-' <unary> | <atom>
std::unique_ptr<ExpressionNode> Parser::unary()
{
    TRACE;
    if (this->eat<TokenType::MINUS>())
        return std::make_unique<NegateNode>(this->unary().release());
    return this->atom();
}

// <atom> = <paren> | <constant> | <id> (<funcargs> | <id>? '=' <expr>)?
std::unique_ptr<ExpressionNode> Parser::atom()
{
    TRACE;
    switch(this->token.type)
    {
        case TokenType::PAREN_OPEN:
            return this->paren();
        case TokenType::ASM:
            return this->assembly();
        case TokenType::INTEGER:
            return this->constant();
        default:
            return this->variable();
    }
}

// <paren> = '(' <expr> ')'
std::unique_ptr<ExpressionNode> Parser::paren()
{
    TRACE;
    this->expect<TokenType::PAREN_OPEN>();
    auto node = std::unique_ptr<ExpressionNode>(expr());
    this->expect<TokenType::PAREN_CLOSE>();
    return node;
}

// <funcargs> = '(' (<expr> (',' <expr>)*) ')'
std::unique_ptr<ArgumentListNode> Parser::funcargs()
{
    TRACE;
    this->expect<TokenType::PAREN_OPEN>();

    if (this->eat<TokenType::PAREN_CLOSE>())
        return std::make_unique<ArgumentListNode>(std::vector<ExpressionNode*>());

    auto args = arglist();
    this->expect<TokenType::PAREN_CLOSE>();
    return args;
}

// <arglist> = <expr> (',' <expr>)*
std::unique_ptr<ArgumentListNode> Parser::arglist()
{
    TRACE;
    std::vector<std::unique_ptr<ExpressionNode>> arguments;

    while (true)
    {
        arguments.push_back(this->expr());

        if (!this->eat<TokenType::COMMA>())
        {
            std::vector<ExpressionNode*> tmp;
            for (auto& arg : arguments)
                tmp.push_back(arg.release());
            return std::make_unique<ArgumentListNode>(tmp);
        }
    }
}

std::unique_ptr<ExpressionNode> Parser::variable()
{
    TRACE;
    if (!this->token.isDataType()) // also includes isType<IDENT>
        this->expected("datatype or identifier");

    Token saved = this->token;
    this->consume();

    switch(this->token.type)
    {
        case TokenType::PAREN_OPEN:
        {
            auto args = this->funcargs();
            return std::make_unique<FunctionCallNode>(saved.lexeme.get<std::string>(), args.release());
        }
        case TokenType::IDENT:
        {
            std::string name = this->token.lexeme.get<std::string>();
            this->consume();

            auto decl = std::make_unique<DeclarationNode>(saved.asDataType(), name);

            if (this->eat<TokenType::EQUALS>())
            {
                auto rhs = this->expr();
                return std::make_unique<AssignmentNode>(decl.release(), rhs.release());
            }

            return decl;
        }
        case TokenType::EQUALS:
        {
            this->consume();
            auto rhs = this->expr();
            VariableNode* name = new VariableNode(saved.lexeme.get<std::string>());
            return std::make_unique<AssignmentNode>(name, rhs.release());
        }
        default:
            return std::make_unique<VariableNode>(saved.lexeme.get<std::string>());
    }
}

std::unique_ptr<ExpressionNode> Parser::constant()
{
    TRACE;
    if (!this->check<TokenType::INTEGER>())
        this->expected(TokenType::INTEGER);

    uint64_t x = this->token.lexeme.get<uint64_t>();
    this->consume();

    if (x <= (1 << 8) -1)
        return std::make_unique<U8ConstantNode>((uint8_t) (x & 0xFF));

    this->error(fmt::sprintf("value of ", x, "overflowed"));
}

std::unique_ptr<AssemblyNode> Parser::assembly()
{
    TRACE;
    this->expect<TokenType::ASM>();
    auto args = this->funcargs();
    this->expect<TokenType::ARROW>();
    auto returntype = this->datatype();
    this->expect<TokenType::BRACE_OPEN>();
    std::string code = this->brainfuck();
    this->expect<TokenType::BRACE_CLOSE>();

    return std::make_unique<AssemblyNode>(returntype.release(), code, args.release());
}

std::string Parser::brainfuck()
{
    TRACE;
    std::stringstream ss;

    while (true)
    {
        switch(this->token.type)
        {
            case TokenType::PLUS:
                ss << "+";
                break;
            case TokenType::MINUS:
                ss << "-";
                break;
            case TokenType::DOT:
                ss << ".";
                break;
            case TokenType::COMMA:
                ss << ",";
                break;
            case TokenType::LEFT:
                ss << "<";
                break;
            case TokenType::RIGHT:
                ss << ">";
                break;
            case TokenType::LEFTLEFT:
                ss << "<<";
                break;
            case TokenType::RIGHTRIGHT:
                ss << ">>";
                break;
            case TokenType::ARROW:
                ss << "->";
                break;
            case TokenType::BRACKET_OPEN:
                this->consume();
                ss << "[" << this->brainfuck();
                if (!this->check<TokenType::BRACKET_CLOSE>())
                    this->expected(TokenType::BRACE_CLOSE);
                ss << "]";
                break;
            default:
                return ss.str();
        }

        this->consume();
    }
}

std::unique_ptr<DataTypeBase> Parser::datatype()
{
    if (!this->token.isDataType())
        this->expected("datatype");
    auto dt = std::unique_ptr<DataTypeBase>(this->token.asDataType());
    this->consume();
    return dt;
}

std::string Parser::ident()
{
    if (!this->check<TokenType::IDENT>())
        this->expected(TokenType::IDENT);

    std::string ident = this->token.lexeme.get<std::string>();
    this->consume();
    return ident;
}

std::unique_ptr<ExpressionNode> Parser::toBinOp(TokenType type, std::unique_ptr<ExpressionNode> lhs, std::unique_ptr<ExpressionNode> rhs)
{
    switch (type)
    {
        case TokenType::PLUS:
            return std::make_unique<AddNode>(lhs.release(), rhs.release());
        case TokenType::MINUS:
            return std::make_unique<SubNode>(lhs.release(), rhs.release());
        case TokenType::STAR:
            return std::make_unique<MulNode>(lhs.release(), rhs.release());
        case TokenType::SLASH:
            return std::make_unique<DivNode>(lhs.release(), rhs.release());
        case TokenType::PERCENT:
            return std::make_unique<ModNode>(lhs.release(), rhs.release());
        case TokenType::AMPERSAND:
            return std::make_unique<BitwiseAndNode>(lhs.release(), rhs.release());
        case TokenType::PIPE:
            return std::make_unique<BitwiseOrNode>(lhs.release(), rhs.release());
        case TokenType::LEFTLEFT:
            return std::make_unique<BitwiseLeftShiftNode>(lhs.release(), rhs.release());
        case TokenType::RIGHTRIGHT:
            return std::make_unique<BitwiseRightShiftNode>(lhs.release(), rhs.release());
        case TokenType::HAT:
            return std::make_unique<BitwiseXorNode>(lhs.release(), rhs.release());
        default:
            throw std::runtime_error("internal error");
    }
}