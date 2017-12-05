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
    if (this->token.isKeyword<Keyword::FUNC>())
        return this->funcdecl();

    if (this->token.isKeyword<Keyword::TYPE>())
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

// <structdecl> = 'type' <id> '{' (<type> <id> (',' <id>)* ',')+ '}'
StructureDefinitionNode* Parser::structdecl()
{
    if (!this->expect<Keyword::TYPE>())
        return nullptr;

    VariableNode* name = variable();
    if (!name)
        return nullptr;

    if (!this->token.isType<TokenType::BRACE_OPEN>())
    {
        delete name;
        return nullptr;
    }

    std::vector<Field> members;
    DataTypeBase *lasttype(nullptr);

    while (true)
    {
        Token saved = this->token;
        if (!this->expect<TokenType::IDENT>())
            break;

        std::string memname;
        DataTypeBase *memtype(nullptr);

        if (this->token.isType<TokenType::COMMA>())
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

        if (this->eat<TokenType::BRACE_CLOSE>())
        {
        	delete lasttype;
        	if (members.size() == 0)
        		return nullptr;

            return new StructureDefinitionNode(name, members);
        }
        else if (!this->expect<TokenType::COMMA>())
            break;
    }

    delete name;
    delete lasttype;
    return nullptr;
}

// <funcdecl> = 'func' <id> <funcpar> ('->' <id>)? <block>
FunctionDeclaration* Parser::funcdecl()
{
    if (!this->expect<Keyword::TYPE>())
        return nullptr;

    VariableNode* name = variable();
    if (!name)
        return nullptr;

    consume();

    FunctionParameters* parameters = funcpar();
    if (!parameters)
        return nullptr;

    DataTypeBase *rtype(nullptr);

    if (this->eat<TokenType::ARROW>())
    {
    	if (!this->token.isDataType())
    	{
    	    delete name;
    		delete parameters;
    		return nullptr;
    	}

    	rtype = this->token.asDataType();
    }
    else
    	rtype = new DataType<DataTypeClass::VOID>();

    BlockNode *list = block();

    if (!list)
    {
        delete name;
        delete parameters;
        delete rtype;
        return nullptr;
    }

    return new FunctionDeclaration(name, parameters, rtype, list);
}

// <funcpar> = '(' (<type> <id> (',' <type>? <id>)*) ')'
FunctionParameters* Parser::funcpar()
{
    if (!this->expect<TokenType::PAREN_OPEN>())
        return nullptr;

    std::vector<Field> parameters;
    DataTypeBase *lasttype(nullptr);

    while (true)
    {
    	Token saved = this->token;
		if (!this->expect<TokenType::IDENT>())
			break;

		std::string parname;
		DataTypeBase *partype(nullptr);

		if (this->token.isType<TokenType::COMMA>())
		{
			if (!lasttype || saved.isReserved())
				break;

			parname = saved.asText();
			partype = lasttype->copy();
			consume();
		}
		else if (this->expect<TokenType::IDENT>())
		{
			if (this->token.isReserved())
				break;

			parname = this->token.asText();
			partype = saved.asDataType();

			delete lasttype;
			lasttype = partype->copy();
		}
		else
			return nullptr;

		parameters.push_back(Field(partype, parname));

		if (this->eat<TokenType::BRACE_CLOSE>())
		{
			delete lasttype;
			return new FunctionParameters(parameters);
		}
		else if (!this->expect<TokenType::COMMA>())
			break;
    }

    return nullptr;
}

// <block> = '{' <statlist> '}'
BlockNode* Parser::block()
{
    if (!this->expect<TokenType::BRACE_OPEN>())
        return statement();

    StatementListNode *list = statlist();

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
	if (this->token.isKeyword<Keyword::IF>())
		return ifstat();

	if (this->token.isKeyword<Keyword::WHILE>())
		return whilestat();

	if (this->token.isKeyword<Keyword::RETURN>())
	    return returnstat();

	return this->exprstat();
}

ReturnNode* Parser::returnstat()
{
    if (!this->expect<Keyword::RETURN>())
        return nullptr;

    ExpressionNode* expr = expr();
    if (!expr)
        return nullptr;

    return new ReturnNode(expr);
}

StatementNode* Parser::exprstat()
{
    ExpressionNode* expr = expr();
    if (!expr)
        return nullptr;

    if (this->eat<TokenType::NEWLINE, TokenType::SEMICOLON>())
        return new ExpressionStatementNode(expr);
    else
        return new ReturnNode(expr);
}

// <ifstat> = 'if' <expr> <block> ('else' (<ifstat> | <block>))?
StatementNode* Parser::ifstat()
{
    if (!this->expect<Keyword::IF>())
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

    if (this->eat<Keyword::ELSE>())
    {
    	StatementNode *alternative(nullptr);
    	if (this->token.isKeyword<Keyword::IF>())
    		alternative = ifstat();
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

// <whilestat> = 'while' <expr> <block>
WhileNode* Parser::whilestat()
{
    if (!this->expect<Keyword::WHILE>())
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
        if (!this->expect<TokenType::PLUS, TokenType::MINUS>())
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
        if (!this->expect<TokenType::STAR, TokenType::SLASH, TokenType::PERCENT>())
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
        }
    }

    return lhs;
}

// <unary> = '-' <unary> | <atom>
ExpressionNode* Parser::unary()
{
    if (this->token.isType<TokenType::MINUS>())
    {
        ExpressionNode *node = unary();
        if (!node)
            return new NegateNode(node);
    }

    return atom();
}

// <atom> = <paren> | <id> (<funcargs> | <id>? '=' <expr>)?
ExpressionNode* Parser::atom()
{
    if (this->token.isType<TokenType::PAREN_OPEN>())
        return paren();

    if (!this->token.isType<TokenType::IDENT>())
        return nullptr;

    Token token = this->token;
    VariableNode *name = variable();

    if (this->token.isType<TokenType::PAREN_OPEN>())
    {
        FunctionArguments *args = funcargs();
        if (!args)
        {
            delete name;
            return nullptr;
        }
        return new FunctionCallNode(name, args);
    }

    if (this->token.isType<TokenType::IDENT>())
    {
        delete name;
        name = variable();
        DataTypeBase *type = token.asDataType();

        if (this->eat<TokenType::EQUALS>())
        {
            ExpressionNode* rhs = expr();
            if (!rhs)
            {
                delete name;
                return nullptr;
            }

            return new AssignmentNode(new DeclarationNode(type, name), rhs);
        }

        return new DeclarationNode(type, name);
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
FunctionArguments* Parser::funcargs()
{
    std::vector<ExpressionNode*> arguments;

    if (!this->expect<TokenType::PAREN_OPEN>())
        return nullptr;

    while (true)
    {
        ExpressionNode *arg = expr();
        if (!arg)
            break;

        arguments.push_back(arg);

        if (this->token.isType<TokenType::PAREN_CLOSE>())
            return new FunctionArguments(arguments);
        else if (!this->expect<TokenType::COMMA>())
            break;
    }

    for (auto expr : arguments)
        delete expr;
    return nullptr;
}

VariableNode* Parser::variable()
{
    if (!this->token.isType<TokenType::IDENT>() || this->token.isReserved())
        return nullptr;
    std::string name = this->token.asText();
    return new VariableNode(name);
}
