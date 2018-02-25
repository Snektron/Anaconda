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

Parser::Parser(std::istream& input):
    lexer(input), token(Span{0, 0}, TokenType::EOI), message("")
{
    consume();
}

void Parser::error(const std::string& msg)
{
    this->message = msg;
}

void Parser::unexpected()
{
    std::stringstream ss;
    fmt::ssprintf(ss, this->token.span, " unexpected token '", this->token, "'");
    if (this->tried.size() > 0)
        fmt::ssprintf(ss, " expected token(s) ", this->tried, '\n');

    this->error(ss.str());
}

GlobalNode* Parser::program()
{
    TRACE;
    return this->prog();
}

const Token& Parser::consume()
{
    this->tried.clear();

    do {
       this->token = this->lexer.next();
    }
    while (this->token.isOneOf<TokenType::WHITESPACE, TokenType::COMMENT, TokenType::NEWLINE>());
    
    std::cout << "consume: " << this->token.type << std::endl;

    return this->token;
}

// <unit> = <globalstat>*
GlobalNode* Parser::prog()
{
    TRACE;
    std::vector<GlobalElementNode*> elements;

    while (!this->check<TokenType::EOI>())
    {
        GlobalElementNode *node = globalstat();
        if (!node)
            return nullptr;

        elements.push_back(node);
    }

    return new GlobalNode(elements);
}

// <globalstat> = <funcdecl> | <structdecl> | <globalexpr>
GlobalElementNode* Parser::globalstat()
{
    TRACE;
    if (this->check<TokenType::FUNC>())
        return this->funcdecl();

    if (this->check<TokenType::TYPE>())
        return this->structdecl();

    return this->globalexpr();
}

GlobalExpressionNode* Parser::globalexpr()
{
    TRACE;
    ExpressionNode* expr = this->expr();
    if (!expr)
        return nullptr;

    if (!this->expect<TokenType::SEMICOLON>())
    {
        delete expr;
        return nullptr;
    }

    return new GlobalExpressionNode(expr);
}

// <structdecl> = 'type' <id> '{' <fieldlist> '}'
StructureDefinitionNode* Parser::structdecl()
{
    TRACE;
    if (!this->expect<TokenType::TYPE>())
        return nullptr;

    if (!this->check<TokenType::IDENT>())
        return nullptr;

    const std::string name = this->token.lexeme.get<std::string>();
    consume();

    if (!this->expect<TokenType::BRACE_OPEN>())
        return nullptr;

    FieldListNode* members = fieldlist();

    if (!this->expect<TokenType::BRACE_CLOSE>())
    {
        delete members;
        return nullptr;
    }

    return new StructureDefinitionNode(name, members);
}

// <funcdecl> = 'func' <id> <funcpar> ('->' <id>)? <block>
FunctionDeclaration* Parser::funcdecl()
{
    TRACE;
    if (!this->expect<TokenType::FUNC>())
        return nullptr;

    if (!this->check<TokenType::IDENT>())
        return nullptr;

    const std::string name = this->token.lexeme.get<std::string>();
    consume();

    FieldListNode* parameters = funcpar();
    if (!parameters)
        return nullptr;

    DataTypeBase *rtype(nullptr);

    if (this->eat<TokenType::ARROW>())
    {
        if (!this->token.isDataType())
        {
            delete parameters;
            return nullptr;
        }

        rtype = this->token.asDataType();
        this->consume();
    }
    else
        rtype = new DataType<DataTypeClass::VOID>();

    BlockNode *body = block();

    if (!body)
    {
        delete parameters;
        delete rtype;
        return nullptr;
    }

    return new FunctionDeclaration(name, parameters, rtype, body);
}

// <funcpar> = '(' <fieldlist> ')'
FieldListNode* Parser::funcpar()
{
    TRACE;
    if (!this->expect<TokenType::PAREN_OPEN>())
        return nullptr;

    FieldListNode* parameters = fieldlist();
    if (!parameters)
        return nullptr;

    if (!this->expect<TokenType::PAREN_CLOSE>())
    {
        delete parameters;
        return nullptr;
    }

    return parameters;
}

// <fieldlist> = (<type> <id> (',' <type>? <id>)*)
FieldListNode* Parser::fieldlist()
{
    TRACE;
    std::vector<Field> parameters;
    DataTypeBase *lasttype(nullptr);

    while (true)
    {
        Token saved = this->token;
        if (!this->token.isDataType()) {
            this->unexpected();
            break;
        }

        this->consume();

        DataTypeBase *partype(nullptr);
        std::string parname;

        if (this->check<TokenType::IDENT>())
        {
            partype = saved.asDataType();
            parname = this->token.lexeme.get<std::string>();
            this->consume();

            delete lasttype;
            lasttype = partype->copy();
        }
        else
        {
            if (!lasttype)
                return nullptr;

            partype = lasttype->copy();
            parname = saved.lexeme.get<std::string>();
        }

        parameters.push_back(Field(partype, parname));

        if (!this->expect<TokenType::COMMA>())
            break;
    }

    delete lasttype;
    return new FieldListNode(parameters);
}

// <block> = '{' <statlist> '}'
BlockNode* Parser::block()
{
    TRACE;
    if (!this->expect<TokenType::BRACE_OPEN>())
        return nullptr;

    StatementNode* list = statlist();

    if (!this->expect<TokenType::BRACE_CLOSE>())
    {
        delete list;
        return nullptr;
    }

    return new BlockNode(list);
}

// <statlist> = <statement>*
StatementNode* Parser::statlist()
{
    TRACE;
    StatementNode *list = new EmptyStatementNode();

    while (true)
    {
        StatementNode *node = statement();
        if (!node)
            break;

        list = new StatementListNode(list, node);
    }

    return list;
}

// <statement> = <ifstat> | <whilestat>
StatementNode* Parser::statement()
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

ReturnNode* Parser::returnstat()
{
    TRACE;
    if (!this->expect<TokenType::RETURN>())
        return nullptr;

    ExpressionNode* expr = this->expr();
    if (!expr)
        return nullptr;

    return new ReturnNode(expr);
}

StatementNode* Parser::exprstat()
{
    TRACE;
    ExpressionNode* expr = this->expr();
    if (!expr)
        return nullptr;

    if (this->eat<TokenType::SEMICOLON>())
        return new ExpressionStatementNode(expr);
    return new ReturnNode(expr);
}

// <ifstat> = 'if' <expr> <block> ('else' (<ifstat> | <block>))?
StatementNode* Parser::ifstat()
{
    TRACE;
    if (!this->expect<TokenType::IF>())
        return nullptr;

    ExpressionNode *condition = expr();
    if (!condition)
        return nullptr;

    StatementNode *consequent = statement();
    if (!consequent)
    {
        delete condition;
        return consequent;
    }

    if (this->eat<TokenType::ELSE>())
    {
        StatementNode *alternative(nullptr);
        if (this->check<TokenType::IF>())
            alternative = ifstat();
        else
            alternative = statement();

        if (!alternative)
        {
            delete condition;
            delete consequent;
            return nullptr;
        }

        return new IfElseNode(condition, consequent, alternative);
    }

    return new IfNode(condition, consequent);
}

// <whilestat> = 'while' <expr> <block>
WhileNode* Parser::whilestat()
{
    TRACE;
    if (!this->expect<TokenType::WHILE>())
        return nullptr;

    ExpressionNode *condition = expr();
    if (!condition)
        return nullptr;

    StatementNode *consequent = statement();
    if (!consequent)
    {
        delete condition;
        return nullptr;
    }

    return new WhileNode(condition, consequent);
}

// <expr> = <sum>
ExpressionNode* Parser::expr()
{
    TRACE;
    return bor();
}

ExpressionNode* Parser::bor()
{
    TRACE;
    ExpressionNode *lhs = bxor();
    if (!lhs)
        return nullptr;

    while (true)
    {
        if (!this->eat<TokenType::PIPE>())
            break;

        ExpressionNode *rhs = bxor();
        if (!rhs)
        {
            delete lhs;
            return nullptr;
        }

        lhs = new BitwiseOrNode(lhs, rhs);
    }

    return lhs;
}

ExpressionNode* Parser::bxor()
{
    TRACE;
    ExpressionNode *lhs = band();
    if (!lhs)
        return nullptr;

    while (true)
    {
        if (!this->eat<TokenType::HAT>())
            break;

        ExpressionNode *rhs = band();
        if (!rhs)
        {
            delete lhs;
            return nullptr;
        }

        lhs = new BitwiseXorNode(lhs, rhs);
    }

    return lhs;
}

ExpressionNode* Parser::band()
{
    TRACE;
    ExpressionNode *lhs = shift();
    if (!lhs)
        return nullptr;

    while (true)
    {
        if (!this->eat<TokenType::AMPERSAND>())
            break;

        ExpressionNode *rhs = shift();
        if (!rhs)
        {
            delete lhs;
            return nullptr;
        }

        lhs = new BitwiseAndNode(lhs, rhs);
    }

    return lhs;
}

ExpressionNode* Parser::shift()
{
    TRACE;
    ExpressionNode *lhs = sum();
    if (!lhs)
        return nullptr;

    while (true)
    {
        TokenType optype = this->token.type;
        if (!this->eatOneOf<TokenType::LEFTLEFT, TokenType::RIGHTRIGHT>())
            break;

        ExpressionNode *rhs = sum();
        if (!rhs)
        {
            delete lhs;
            return nullptr;
        }

        if (optype == TokenType::LEFTLEFT)
            lhs = new BitwiseLeftShiftNode(lhs, rhs);
        else
            lhs = new BitwiseRightShiftNode(lhs, rhs);
    }

    return lhs;
}

// <sum> = <product> (('+' | '-') <product>)*
ExpressionNode* Parser::sum()
{
    TRACE;
    ExpressionNode *lhs = product();
    if (!lhs)
        return nullptr;

    while (true)
    {
        TokenType optype = this->token.type;
        if (!this->eatOneOf<TokenType::PLUS, TokenType::MINUS>())
            break;

        ExpressionNode *rhs = product();
        if (!rhs)
        {
            delete lhs;
            return nullptr;
        }

        if (optype == TokenType::PLUS)
            lhs = new AddNode(lhs, rhs);
        else
            lhs = new SubNode(lhs, rhs);
    }

    return lhs;
}

// <product> = <unary> (('*' | '/' | '%') <unary>)*
ExpressionNode* Parser::product()
{
    TRACE;
    ExpressionNode *lhs = unary();
    if (!lhs)
        return nullptr;

    while (true)
    {
        TokenType optype = this->token.type;
        if (!this->eatOneOf<TokenType::STAR, TokenType::SLASH, TokenType::PERCENT>())
            break;

        ExpressionNode *rhs = unary();
        if (!rhs)
        {
            delete lhs;
            return nullptr;
        }

        if (optype == TokenType::STAR)
            lhs = new MulNode(lhs, rhs);
        else if (optype == TokenType::SLASH)
            lhs = new DivNode(lhs, rhs);
        else
            lhs = new ModNode(lhs, rhs);
    }

    return lhs;
}

// <unary> = '-' <unary> | <atom>
ExpressionNode* Parser::unary()
{
    TRACE;
    if (this->eat<TokenType::MINUS>())
    {
        ExpressionNode *node = unary();
        if (!node)
            return new NegateNode(node);
    }

    return atom();
}

// <atom> = <paren> | <constant> | <id> (<funcargs> | <id>? '=' <expr>)?
ExpressionNode* Parser::atom()
{
    TRACE;
    switch(this->token.type)
    {
        case TokenType::PAREN_OPEN:
            return paren();
        case TokenType::ASM:
            return assembly();
        case TokenType::INTEGER:
            return constant();
        default:
            return variable();
    }
}

// <paren> = '(' <expr> ')'
ExpressionNode* Parser::paren()
{
    TRACE;
    if (!this->expect<TokenType::PAREN_OPEN>())
        return nullptr;

    ExpressionNode *node = expr();
    if (!node)
        return nullptr;

    if (!this->expect<TokenType::PAREN_CLOSE>())
    {
        delete node;
        return nullptr;
    }

    return node;
}

// <funcargs> = '(' (<expr> (',' <expr>)*) ')'
ArgumentListNode* Parser::funcargs()
{
    TRACE;
    if (!this->expect<TokenType::PAREN_OPEN>())
        return nullptr;

    if (this->eat<TokenType::PAREN_CLOSE>()) {
        std::vector<ExpressionNode*> arguments;
        return new ArgumentListNode(arguments);
    }

    ArgumentListNode* args = arglist();
    if (!args)
        return nullptr;

    if (!this->expect<TokenType::PAREN_CLOSE>())
    {
        delete args;
        return nullptr;
    }

    return args;
}

// <arglist> = <expr> (',' <expr>)*
ArgumentListNode* Parser::arglist()
{
    TRACE;
    std::vector<ExpressionNode*> arguments;

    while (true)
    {
        ExpressionNode *arg = expr();
        if (!arg)
            break;

        arguments.push_back(arg);

        if (!this->expect<TokenType::COMMA>())
            return new ArgumentListNode(arguments);
    }

    for (auto expr : arguments)
        delete expr;
    return nullptr;
}

ExpressionNode* Parser::variable()
{
    TRACE;
    if (!this->token.isDataType())
        return nullptr;

    Token saved = this->token;
    this->consume();

    switch(this->token.type)
    {
        case TokenType::PAREN_OPEN:
        {
            ArgumentListNode *args = funcargs();
            if (!args)
                return nullptr;
            return new FunctionCallNode(saved.lexeme.get<std::string>(), args);
        }
        case TokenType::IDENT:
        {
            DataTypeBase *type = saved.asDataType();
            if (!type)
                return nullptr;
            std::string name = this->token.lexeme.get<std::string>();
            this->consume();

            if (this->eat<TokenType::EQUALS>())
            {
                ExpressionNode* rhs = expr();
                if (!rhs) {
                    delete type;
                    return nullptr;
                }

                return new AssignmentNode(new DeclarationNode(type, name), rhs);
            }

            return new DeclarationNode(type, name);
        }
        case TokenType::EQUALS:
        {
            this->consume();
            ExpressionNode* rhs = expr();
            if (!rhs)
                return nullptr;
            VariableNode* name = new VariableNode(saved.lexeme.get<std::string>());
            return new AssignmentNode(name, rhs);
        }
        default:
            return new VariableNode(saved.lexeme.get<std::string>());
    }
}

ExpressionNode* Parser::constant()
{
    TRACE;
    if (!this->check<TokenType::INTEGER>())
        return nullptr;
    uint64_t x = this->token.lexeme.get<uint64_t>();
    this->consume();

    if (x <= (1 << 8) -1)
        return new U8ConstantNode((uint8_t) (x & 0xFF));

    this->error(fmt::sprintf("Error: Value of ", x, " overflowed."));
    return nullptr;
}

AssemblyNode* Parser::assembly()
{
    TRACE;
    if (!this->expect<TokenType::ASM>())
        return nullptr;

    ArgumentListNode* args = funcargs();
    if (!args)
        return nullptr;

    if (!this->expect<TokenType::ARROW>() || !this->token.isDataType())
    {
        delete args;
        return nullptr;
    }

    DataTypeBase* rtype = this->token.asDataType();
    this->consume();

    if (!this->expect<TokenType::BRACE_OPEN>())
    {
        delete args;
        delete rtype;
        return nullptr;
    }

    std::string* code = brainfuck();
    if (!code)
    {
        delete code;
        delete args;
        delete rtype;
        return nullptr;
    }

    return new AssemblyNode(rtype, *code, args);
}

std::string* Parser::brainfuck()
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
                {
                    std::string* str = brainfuck();
                    ss << "[" << *str;
                    delete str;
                }

                if (!this->expect<TokenType::BRACKET_CLOSE>())
                    return nullptr;

                ss << "]";
                break;
            default:
                return new std::string(ss.str());
        }

        this->consume();
    }
}
