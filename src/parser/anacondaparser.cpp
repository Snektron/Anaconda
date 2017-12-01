#include "parser/anacondaparser.h"

AnacondaParser::AnacondaParser(const std::string& input):
	Parser(input) {}

// <parse> = <unit> <WS>? <EOF>
StatementListNode* AnacondaParser::parse()
{
	StatementListNode *node = unit();
	whitespace();
	if (!atEnd())
	{
		delete node;
		return nullptr;
	}
	return node;
}

std::string AnacondaParser::name()
{
	beginCapture();
	if (!expectLetter())
	{
		endCapture();
		return "";
	}

	while(expectLetter() || expect('_'));

	return endCapture();
}

// <unit> = <funcdecl>*
StatementListNode* AnacondaParser::unit()
{
	StatementListNode *list = nullptr;

	while (true)
	{
		Node *node = funcdecl();
		if (!node)
			break;

		list = new StatementListNode(list, node);
	}

	return list;
}

// <funcdecl> = <WS>? 'func' <WS> <name> <WS>? '(' <funcpar> ')' ('->' <name>)? <block>
FunctionDeclaration* AnacondaParser::funcdecl()
{
	whitespace();
	if (!(expect("func") && whitespace()))
		return nullptr;

	std::string funcname = name();
	if (funcname == "")
		return nullptr;

	whitespace();
	if (!expect('('))
		return nullptr;

	FunctionArgumentDeclarationNode* args = funcpar();

	whitespace();
	if (!expect(')'))
	{
		delete args;
		return nullptr;
	}

	std::string returntype("");

	whitespace();
	if (expect("->")) {
		whitespace();
		returntype = name();
	}

	StatementListNode *list = block();

	return nullptr; // TODO: return new FunctionDeclaration
}

// <funcpar> = (<WS>? <name> <WS>? ',')* <name>?
FunctionArgumentDeclarationNode* AnacondaParser::funcpar()
{
	FunctionArgumentDeclarationNode *list = nullptr;

	while (true)
	{
		whitespace();
		std::string parname = name();
		if (parname == "")
			break;

		list = new FunctionArgumentDeclarationNode(parname, list);

		whitespace();
		if (!expect(','))
			break;
	}

	return list;
}

// <block> = '{' <statlist> '}'
StatementListNode* AnacondaParser::block()
{
	whitespace();
	if (!expect('{'))
		return nullptr;

	StatementListNode *list = statlist();

	whitespace();
	if (!expect('}'))
	{
		delete list;
		return nullptr;
	}

	return list;
}

// <statlist> = <statement>*
StatementListNode* AnacondaParser::statlist()
{
	StatementListNode *list = nullptr;

	while (true)
	{
		Node *node = statement();
		if (!node)
			break;

		list = new StatementListNode(list, node);
	}

	return list;
}

// <statement> = <ifstat>
Node* AnacondaParser::statement()
{
	return ifstat();
}

Node* AnacondaParser::ifstat()
{
	whitespace();
	if (!(expect("if") && !whitespace()))
		return nullptr;

	return nullptr;
}

// <expr> = <sum>
ExpressionNode* AnacondaParser::expr()
{
	return sum();
}

// <sum> = <product> (<WS>? ('+' | '-') <product>)*
ExpressionNode* AnacondaParser::sum()
{
	ExpressionNode *lhs = product();
	if (!lhs)
		return nullptr;

	while (true)
	{
		whitespace();

		char op = peek();
		if (op != '+' && op != '-')
			break;

		Node *rhs = product();
		if (!rhs)
		{
			delete lhs;
			return nullptr;
		}

		switch (op)
		{
		case '+':
			lhs = new AddNode(lhs, rhs);
			break;
		case '-':
			lhs = new SubNode(lhs, rhs);
			break;
		}
	}

	return lhs;
}

// <product> = <unary> (<WS>? ('*' | '/' | '%') <unary>)*
ExpressionNode* AnacondaParser::product()
{
	ExpressionNode *lhs = unary();
	if (!lhs)
		return nullptr;

	while (true)
	{
		whitespace();

		char op = peek();
		if (op != '*' && op != '/' && op != '%')
			break;

		Node *rhs = unary();
		if (!rhs)
		{
			delete lhs;
			return nullptr;
		}

		switch (op)
		{
		case '*':
			lhs = new MulNode(lhs, rhs);
			break;
		case '/':
			lhs = new DivNode(lhs, rhs);
			break;
		case '%':
			lhs = new ModNode(lhs, rhs);
			break;
		}
	}

	return nullptr;
}

// <unary> = <WS>? ('-' <unary> | <atom>)
ExpressionNode* AnacondaParser::unary()
{
	whitespace();
	if (expect('-'))
	{
		Node *node = unary();
		if (!node)
			(void) 0; // TODO: return new UnaryMinusNode(node);
	}

	return atom();
}

// <atom> = <paren> | <funccall> | <variable>
ExpressionNode* AnacondaParser::atom()
{
	state_t state = save();

	Node *node = paren();
	if (node)
		return node;
	restore(state);

	Node *node = funccall();
	if (node)
		return node;
	restore(state);

	node = variable();
	if (node)
		return node;

	return nullptr;
}

// <paren> = <WS>? '(' <expr> <WS>? ')'
ExpressionNode* AnacondaParser::paren()
{
	whitespace();
	if (!expect('('))
		return nullptr;

	Node *node = expr();
	if (!node)
		return nullptr;

	whitespace();
	if (!expect(')'))
	{
		delete node;
		return nullptr;
	}

	return node;
}

// <funccall> = <WS>? <name> <WS>? '(' <funcargs> <WS>? ')'
FunctionCallNode* AnacondaParser::funccall()
{
	whitespace();
	std::string funcname = name();
	if (funcname == "")
		return nullptr;

	whitespace();
	if (!expect('('))
		return nullptr;

	FunctionArgumentNode* args = funcargs();

	whitespace();
	if (!expect(')'))
	{
		delete args;
		return nullptr;
	}

	return new FunctionCallNode(funcname, args);
}

// <funcargs> = (<WS>? <expr> <WS>? ',')* <expr>?
FunctionArgumentNode* AnacondaParser::funcargs()
{
	FunctionArgumentDeclarationNode *list = nullptr;

	while (true)
	{
		whitespace();
		ExpressionNode *arg = expr();
		if (!arg)
			break;

		list = new FunctionArgumentNode(arg, list);

		whitespace();
		if (!expect(','))
			break;
	}

	return list;
}

// <variable> = <name>
VariableNode* AnacondaParser::variable()
{
	whitespace();
	std::string varname = name();
	if (varname == "")
		return nullptr;

	return new VariableNode(varname);
}

