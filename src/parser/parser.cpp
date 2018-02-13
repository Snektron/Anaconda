#include "parser/parser.h"
#include "common/format.h"
#include <sstream>

Parser::Parser(std::istream& input):
    lexer(input), token(Span{0, 0}, TokenType::EOI)
{
    consume();
}

void Parser::error(const std::string& msg)
{
    this->messages.push_back(Message(MessageType::ERROR, msg));
}

void Parser::unexpected()
{
    std::stringstream ss;
    fmt::ssprintf(ss, "Error: unexpected token '", this->token, "'");
    if (this->tried.size() > 0)
        fmt::ssprintf(ss, " expected token(s) ", this->tried, '\n');

    this->error(ss.str());
}

GlobalNode* Parser::program()
{
    GlobalNode* node = this->prog();
    if (!this->expect<TokenType::EOI>())
    {
        delete node;
        return nullptr;
    }

    return node;
}

const Token& Parser::consume()
{
    this->tried.clear();

    do
       this->token = this->lexer.next();
    while (this->token.isOneOf<TokenType::WHITESPACE, TokenType::COMMENT, TokenType::NEWLINE>());

    return this->token;
}

// <unit> = <globalstat>*
GlobalNode* Parser::prog()
{
    std::vector<GlobalElementNode*> elements;

    while (true)
    {
        GlobalElementNode *node = globalstat();
        if (!node)
            break;

        elements.push_back(node);
    }

    return new GlobalNode(elements);
}

// <globalstat> = <funcdecl> | <structdecl> | <globalexpr>
GlobalElementNode* Parser::globalstat()
{
    if (this->check<TokenType::FUNC>())
        return this->funcdecl();

    if (this->check<TokenType::TYPE>())
        return this->structdecl();

    return this->globalexpr();
}

GlobalExpressionNode* Parser::globalexpr()
{
    ExpressionNode* expr = this->expr();
    if (!expr)
        return nullptr;
    return new GlobalExpressionNode(expr);
}

// <structdecl> = 'type' <id> '{' <fieldlist> '}'
StructureDefinitionNode* Parser::structdecl()
{
    if (!this->expect<TokenType::TYPE>())
        return nullptr;

    if (!this->check<TokenType::IDENT>())
    {
        this->unexpected();
        return nullptr;
    }

    const std::string& name = this->token.lexeme.get<std::string>();
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
    if (!this->expect<TokenType::FUNC>())
        return nullptr;

    if (!this->check<TokenType::IDENT>() || this->token.isReserved())
        return nullptr;

    const std::string& name = this->token.lexeme.get<std::string>();
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
    std::vector<Field> parameters;
    DataTypeBase *lasttype(nullptr);

    while (true)
    {
        Token saved = this->token;
        if (!this->expect<TokenType::IDENT>())
            break;

        std::string parname;
        DataTypeBase *partype(nullptr);

        if (this->eat<TokenType::COMMA>())
        {
            if (!lasttype || saved.isReserved())
                break;

            parname = saved.lexeme.get<std::string>();
            partype = lasttype->copy();
        }
        else if (this->expect<TokenType::IDENT>())
        {
            parname = this->token.lexeme.get<std::string>();
            partype = saved.asDataType();

            delete lasttype;
            lasttype = partype->copy();
        }
        else
            return nullptr;

        parameters.push_back(Field(partype, parname));


        if (!this->expect<TokenType::COMMA>())
        {
            delete lasttype;
            return new FieldListNode(parameters);
        }
    }

    unexpected();
    delete lasttype;
    return nullptr;
}

// <block> = '{' <statlist> '}'
BlockNode* Parser::block()
{
    if (!this->expect<TokenType::BRACE_OPEN>())
        return nullptr;

    StatementListNode* list = statlist();

    if (!this->expect<TokenType::BRACE_CLOSE>())
    {
        delete list;
        return nullptr;
    }

    return new BlockNode(list);
}

// <statlist> = <statement>*
StatementListNode* Parser::statlist()
{
    StatementListNode *list = nullptr;

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
    if (!this->expect<TokenType::RETURN>())
        return nullptr;

    ExpressionNode* expr = this->expr();
    if (!expr)
        return nullptr;

    return new ReturnNode(expr);
}

StatementNode* Parser::exprstat()
{
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
    return sum();
}

// <sum> = <product> (('+' | '-') <product>)*
ExpressionNode* Parser::sum()
{
    ExpressionNode *lhs = product();
    if (!lhs)
        return nullptr;

    while (true)
    {
        TokenType optype = this->token.type;
        if (!this->eat<TokenType::PLUS>() ||
            !this->eat<TokenType::MINUS>())
            break;

        ExpressionNode *rhs = product();
        if (!rhs)
        {
            delete lhs;
            return nullptr;
        }

        switch (optype)
        {
            case TokenType::PLUS:
                lhs = new AddNode(lhs, rhs);
                break;
            case TokenType::MINUS:
                lhs = new SubNode(lhs, rhs);
                break;
            default:
                unexpected();
        }
    }

    return lhs;
}

// <product> = <unary> (('*' | '/' | '%') <unary>)*
ExpressionNode* Parser::product()
{
    ExpressionNode *lhs = unary();
    if (!lhs)
        return nullptr;

    while (true)
    {
        TokenType optype = this->token.type;
        if (!this->eat<TokenType::STAR>() ||
            !this->eat<TokenType::SLASH>() ||
            !this->eat<TokenType::PERCENT>())
            break;
        consume();

        ExpressionNode *rhs = unary();
        if (!rhs)
        {
            delete lhs;
            return nullptr;
        }

        switch (optype)
        {
            case TokenType::STAR:
                lhs = new MulNode(lhs, rhs);
                break;
            case TokenType::SLASH:
                lhs = new DivNode(lhs, rhs);
                break;
            case TokenType::PERCENT:
                lhs = new ModNode(lhs, rhs);
                break;
            default:
                unexpected();
        }
    }

    return lhs;
}

// <unary> = '-' <unary> | <atom>
ExpressionNode* Parser::unary()
{
    if (this->check<TokenType::MINUS>())
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
    if (this->check<TokenType::PAREN_OPEN>())
        return paren();

    if (this->check<TokenType::INTEGER>())
        return constant();

    if (!this->check<TokenType::IDENT>())
        return nullptr;

    Token token = this->token;
    VariableNode *name = variable();

    if (this->check<TokenType::PAREN_OPEN>())
    {
        delete name;
        if (token.isReserved())
            return nullptr;

        ArgumentListNode *args = funcargs();
            return nullptr;
        return new FunctionCallNode(token.lexeme.get<std::string>(), args);
    }

    if (this->check<TokenType::IDENT>())
    {
        delete name;
        if (this->token.isReserved())
            return nullptr;
        const std::string& tname = this->token.lexeme.get<std::string>();
        consume();
        DataTypeBase *type = token.asDataType();

        if (this->eat<TokenType::EQUALS>())
        {
            ExpressionNode* rhs = expr();
            if (!rhs)
                return nullptr;

            return new AssignmentNode(new DeclarationNode(type, tname), rhs);
        }

        return new DeclarationNode(type, tname);
    }

    if (this->eat<TokenType::EQUALS>())
    {
        ExpressionNode* rhs = expr();
        if (!rhs)
        {
            delete name;
            return nullptr;
        }

        return new AssignmentNode(name, rhs);
    }

    return name;
}

// <paren> = '(' <expr> ')'
ExpressionNode* Parser::paren()
{
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
    if (!this->expect<TokenType::PAREN_OPEN>())
        return nullptr;

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

// <arglist> = (<expr> (',' <expr>)*)?
ArgumentListNode* Parser::arglist()
{
    std::vector<ExpressionNode*> arguments;

    while (true)
    {
        ExpressionNode *arg = expr();
        if (!arg)
            break;

        arguments.push_back(arg);

        if (!this->check<TokenType::COMMA>())
            return new ArgumentListNode(arguments);
    }

    for (auto expr : arguments)
        delete expr;
    return nullptr;
}

VariableNode* Parser::variable()
{
    if (!this->check<TokenType::IDENT>())
        return nullptr;
    return new VariableNode(this->token.lexeme.get<std::string>());
}

ExpressionNode* Parser::constant()
{
    if (this->check<TokenType::INTEGER>())
        return nullptr;


    
    
    return nullptr;
}
