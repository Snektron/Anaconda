#include "parser/parser.h"
#include <iostream>

Parser::Parser(std::istream& input):
    lexer(input), token(nextFiltered()) {}

Token Parser::nextFiltered()
{
    Token token(TokenType::UNKNOWN);

    do
        token = this->lexer.next();
    while (token.isOneOf<TokenType::WHITESPACE, TokenType::COMMENT>());

    return token;
}

// placeholder
void Parser::unexpected()
{
    std::cout << "Unexpected token " << this->token.type << std::endl;
}

const Token& Parser::consume()
{
    this->token = nextFiltered();
    return this->token;
}

// <parse> = <unit> <EOF>
GlobalNode* Parser::program()
{
    GlobalNode *node = prog();

    if (!isAtEnd())
    {
        delete node;
        return nullptr;
    }
    return node;
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

// <globalstat> = <funcdecl> | <structdecl> | <declstat>
GlobalElementNode* Parser::globalstat()
{
    if (this->check<Keyword::FUNC>())
        return this->funcdecl();
    else if (this->check<Keyword::TYPE>())
        return this->structdecl();
    else
        return this->declstat<GlobalDeclarationNode>();
}

// <structdecl> = 'type' <id> '{' (<type> <id> (',' <id>)* ',')+ '}'
StructureDefinitionNode* Parser::structdecl()
{
    if (!this->expect<Keyword::TYPE>() || !this->check<TokenType::IDENT>() || this->token.isReserved())
        return nullptr;

    std::string name = this->token.asText();

    if (!this->check<TokenType::BRACE_OPEN>())
        return nullptr;

    std::vector<Field> members;
    DataTypeBase *lasttype(nullptr);

    while (true)
    {
        Token saved = this->token;
        if (!this->expect<TokenType::IDENT>())
            break;

        std::string memname;
        DataTypeBase *memtype(nullptr);

        if (this->check<TokenType::COMMA>())
        {
            if (!lasttype || saved.isReserved())
                break;

            memname = saved.asText();
            memtype = lasttype->copy();
            consume();
        }
        else if (this->expect<TokenType::IDENT>())
        {
            if (this->token.isReserved())
                break;

            memname = this->token.asText();
            memtype = saved.asDataType();

            delete lasttype;
            lasttype = memtype->copy();
        }
        else
            return nullptr;

        members.push_back(Field(memtype, memname));

        if (this->check<TokenType::BRACE_CLOSE>())
        {
            consume();
            return new StructureDefinitionNode(name, members);
        }
        else if (!this->expect<TokenType::COMMA>())
            break;
    }

    delete lasttype;
    return nullptr;
}

// <funcdecl> = <WS>? 'func' <WS> <id> <WS>? <funcpar> ('->' <id>)? <block>
FunctionDeclaration* Parser::funcdecl()
{
    whitespace();
    if (!expect(KEYWORD_FUNC) || !whitespace())
        return nullptr;

    std::string name = id();
    if (name == "")
        return nullptr;

    FunctionParameters* parameters = funcpar();
    if (!parameters)
        return nullptr;

    whitespace();
    DataTypeBase *rtype(expect(TOKEN_ARROW) ? type() : new DataType<DataTypeClass::VOID>);

    if (!rtype)
    {
        delete parameters;
        return nullptr;
    }

    BlockNode *list = block();

    if (!list)
    {
        delete parameters;
        delete rtype;
        return nullptr;
    }

    return new FunctionDeclaration(name, parameters, rtype, list);
}

// <funcpar> = <WS>? '(' ((<type> <WS>)? <id> (',' (<type> <WS>)? <id>)*) <WS>? ')'
FunctionParameters* Parser::funcpar()
{
    std::vector<Field> parameters;
    DataTypeBase *lasttype(nullptr);

    whitespace();
    if (!expect(TOKEN_BRACE_OPEN))
        return nullptr;

    while (true)
    {
        State state = save();

        whitespace();
        DataTypeBase *partype(type());
        std::string parname;

        if (!partype || !whitespace() || (parname = id()) == "")
        {
            restore(state);
            delete partype;

            parname = id();
            if (!lasttype || parname == "")
            {
                delete lasttype;
                break;
            }

            partype = lasttype->copy();
        }
        else
            lasttype = partype->copy();

        parameters.push_back(Field(partype, parname));

        whitespace();
        if (expect(TOKEN_BRACE_CLOSE))
            return new FunctionParameters(parameters);
        else if (!expect(TOKEN_COMMA))
            break;
    }

    return nullptr;
}

// <block> = '{' <statlist> '}'
BlockNode* Parser::block()
{
    whitespace();
    if (!expect(TOKEN_BRACE_OPEN))
        return nullptr;

    StatementListNode *list = statlist();

    whitespace();
    if (!expect(TOKEN_BRACE_CLOSE))
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

// <statement> = <ifstat> | <whilestat> | <declstat> | <assignstat>
StatementNode* Parser::statement()
{
    State state = save();

    StatementNode *node = ifstat();
    if (node)
        return node;

    restore(state);
    node = whilestat();
    if (node)
        return node;

    restore(state);
    node = declstat<DeclarationNode>();
    if (node)
        return node;

    restore(state);
    return assignstat();
}

// <ifstat> = <WS>? 'if' <WS> <expr> <block> ('else' (<WS> <ifstat> | <block>))?
StatementNode* Parser::ifstat()
{
    whitespace();
    if (!expect(KEYWORD_IF) || !whitespace())
        return nullptr;

    ExpressionNode *condition = expr();
    if (!condition)
        return nullptr;

    BlockNode *consequent = block();
    if (!consequent)
    {
        delete condition;
        return consequent;
    }

    if (expect(KEYWORD_ELSE))
    {
        StatementNode *alternative(nullptr);
        State state = save();

        if (whitespace() && expect(KEYWORD_IF))
        {
            restore(state);
            alternative = ifstat();
        }
        else
            alternative = block();

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

// <whilestat> = <WS>? 'while' <WS> <expr> <block>
WhileNode* Parser::whilestat()
{
    whitespace();
    if (!expect(KEYWORD_WHILE) || !whitespace())
        return nullptr;

    ExpressionNode *condition = expr();
    if (!condition)
        return nullptr;

    BlockNode *consequent = block();
    if (!consequent)
    {
        delete condition;
        return nullptr;
    }

    return new WhileNode(condition, consequent);
}

// <assignstat> = <WS>? <id> <WS>? '=' <WS>? <expr>
AssignmentNode* Parser::assignstat()
{
    whitespace();
    std::string name = id();
    if (name == "")
        return nullptr;

    ExpressionNode *assignment = expr();
    if (!assignment)
        return nullptr;

    return new AssignmentNode(name, assignment);
}

// <declstat> = <WS>? <type> <WS> <id> <WS>? ('=' <WS>? <expr>)?
template <typename T>
T* Parser::declstat()
{
    whitespace();
    DataTypeBase* dtype = type();
    std::string name;

    if (!dtype || !whitespace() || (name = id()) == "")
    {
        delete dtype;
        return nullptr;
    }

    whitespace();
    if (!expect(TOKEN_EQUALS))
        return new T(dtype, name);

    whitespace();
    ExpressionNode *value = expr();
    if (!value)
    {
        delete dtype;
        return nullptr;
    }

    return new T(dtype, name, value);
}

// <expr> = <sum>
ExpressionNode* Parser::expr()
{
    return sum();
}

// <sum> = <product> (<WS>? ('+' | '-') <product>)*
ExpressionNode* Parser::sum()
{
    ExpressionNode *lhs = product();
    if (!lhs)
        return nullptr;

    while (true)
    {
        whitespace();

        char op = peek();
        if (op != TOKEN_PLUS && op != TOKEN_MINUS)
            break;

        ExpressionNode *rhs = product();
        if (!rhs)
        {
            delete lhs;
            return nullptr;
        }

        switch (op)
        {
            case TOKEN_PLUS:
                lhs = new AddNode(lhs, rhs);
                break;
            case TOKEN_MINUS:
                lhs = new SubNode(lhs, rhs);
                break;
        }
    }

    return lhs;
}

// <product> = <unary> (<WS>? ('*' | '/' | '%') <unary>)*
ExpressionNode* Parser::product()
{
    ExpressionNode *lhs = unary();
    if (!lhs)
        return nullptr;

    while (true)
    {
        whitespace();

        char op = peek();
        if (op != TOKEN_STAR && op != TOKEN_SLASH && op != TOKEN_PERCENT)
            break;

        ExpressionNode *rhs = unary();
        if (!rhs)
        {
            delete lhs;
            return nullptr;
        }

        switch (op)
        {
            case TOKEN_STAR:
                lhs = new MulNode(lhs, rhs);
                break;
            case TOKEN_SLASH:
                lhs = new DivNode(lhs, rhs);
                break;
            case TOKEN_PERCENT:
                lhs = new ModNode(lhs, rhs);
                break;
        }
    }

    return lhs;
}

// <unary> = <WS>? ('-' <unary> | <atom>)
ExpressionNode* Parser::unary()
{
    whitespace();
    if (expect('-'))
    {
        ExpressionNode *node = unary();
        if (!node)
            return new NegateNode(node);
    }

    return atom();
}

// <atom> = <paren> | <funccall> | <variable>
ExpressionNode* Parser::atom()
{
    State state = save();

    ExpressionNode *node = paren();
    if (node)
        return node;

    restore(state);
    node = funccall();
    if (node)
        return node;

    restore(state);
    return variable();
}

// <paren> = <WS>? '(' <expr> <WS>? ')'
ExpressionNode* Parser::paren()
{
    whitespace();
    if (!expect(TOKEN_PAREN_OPEN))
        return nullptr;

    ExpressionNode *node = expr();
    if (!node)
        return nullptr;

    whitespace();
    if (!expect(TOKEN_PAREN_CLOSE))
    {
        delete node;
        return nullptr;
    }

    return node;
}

// <funccall> = <WS>? <id> <WS>? '(' <funcargs> <WS>? ')'
FunctionCallNode* Parser::funccall()
{
    whitespace();
    std::string name = id();
    if (name == "")
        return nullptr;

    whitespace();
    if (!expect(TOKEN_PAREN_OPEN))
        return nullptr;

    FunctionArguments* args = funcargs();

    whitespace();
    if (!expect(TOKEN_PAREN_CLOSE))
    {
        delete args;
        return nullptr;
    }

    return new FunctionCallNode(name, args);
}

// <funcargs> = <WS>? '(' (<expr> (',' <expr>)*) <WS>? ')'
FunctionArguments* Parser::funcargs()
{
    std::vector<ExpressionNode*> arguments;

    whitespace();
    if (!expect(TOKEN_PAREN_OPEN))
        return nullptr;

    while (true)
    {
        whitespace();
        ExpressionNode *arg = expr();
        if (!arg)
            break;

        arguments.push_back(arg);

        whitespace();
        if (expect(TOKEN_PAREN_CLOSE))
            return new FunctionArguments(arguments);
        else if (!expect(TOKEN_COMMA))
            break;
    }

    for (auto expr : arguments)
        delete expr;
    return nullptr;
}

// <variable> = <id>
VariableNode* Parser::variable()
{
    if (this->token.isType<TokenType::IDENT>())
        new VariableNode(this->token.asText());

    return nullptr;
}
