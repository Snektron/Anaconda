#include "parser/anacondaparser.h"
#include "ast/datatype.h"

AnacondaParser::AnacondaParser(const std::string& input):
	Parser(input) {}

// <parse> = <unit> <WS>? <EOF>
GlobalNode* AnacondaParser::parse()
{
	GlobalNode *node = unit();
	whitespace();
	if (!atEnd())
	{
		delete node;
		return nullptr;
	}
	return node;
}

std::string AnacondaParser::id()
{
	beginCapture();
	if (!expectLetter())
	{
		endCapture();
		return "";
	}

	while(expectLetter() || expect('_'))
		continue;

	return endCapture();
}

DataTypeBase* AnacondaParser::type() {
	whitespace();

	if (expect("u8"))
		return new DataType<DataTypeClass::U8>();
	else if (expect("void"))
		return new DataType<DataTypeClass::VOID>();
	else
	{
		std::string name = id();
		if (name == "")
			return nullptr;

		return new DataType<DataTypeClass::STRUCT_FORWARD>(name);
	}

	return nullptr;
}

// <unit> = <funcdecl>*
GlobalNode* AnacondaParser::unit()
{
	std::vector<GlobalElementNode*> elements;

	while (true)
	{
		GlobalElementNode *node = (GlobalElementNode*) funcdecl();
		if (!node)
			break;

		elements.push_back(node);
	}

	return new GlobalNode(elements);
}

// <funcdecl> = <WS>? 'func' <WS> <id> <WS>? '(' <funcpar> ')' ('->' <id>)? <block>
FunctionDeclaration* AnacondaParser::funcdecl()
{
	whitespace();
	if (!(expect("func") && whitespace()))
		return nullptr;

	std::string name = id();
	if (name == "")
		return nullptr;

	FunctionParameters* parameters = funcpar();
	if (!parameters)
		return nullptr;

	whitespace();
	DataTypeBase *rtype(expect("->") ? type() : new DataType<DataTypeClass::VOID>);

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
FunctionParameters* AnacondaParser::funcpar()
{
	std::map<std::string, DataTypeBase*> parameters;
	DataTypeBase *lasttype(nullptr);

	whitespace();
	if (!expect('('))
		return nullptr;

	while (true)
	{
		state_t state = save();

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

			//TODO partype = copy(lasttype)
		}
		else
		{
			//TODO lasttype = copy(partype)
		}

		parameters[parname] = partype;

		whitespace();
		if (expect(')'))
			return new FunctionParameters(parameters);
		else if (!expect(','))
			break;
	}

	for (auto i : parameters)
		delete i.second;
	return nullptr;
}

// <block> = '{' <statlist> '}'
BlockNode* AnacondaParser::block()
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

	return new BlockNode(list);
}

// <statlist> = <statement>*
StatementListNode* AnacondaParser::statlist()
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
StatementNode* AnacondaParser::statement()
{
	state_t state = save();

	StatementNode *node = ifstat();
	if (node)
		return node;

	restore(state);
	return whilestat();
}

// <ifstat> = <WS>? 'if' <WS> <expr> <block> ('else' (<WS> <ifstat> | <block>))?
StatementNode* AnacondaParser::ifstat()
{
	whitespace();
	if (!expect("if") || !whitespace())
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

	if (expect("else"))
	{
		StatementNode *alternative(nullptr);
		state_t state = save();

		if (whitespace() && expect("if"))
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
WhileNode* AnacondaParser::whilestat()
{
	whitespace();
	if (!expect("while") || !whitespace())
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

		ExpressionNode *rhs = product();
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

		ExpressionNode *rhs = unary();
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

	return lhs;
}

// <unary> = <WS>? ('-' <unary> | <atom>)
ExpressionNode* AnacondaParser::unary()
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
ExpressionNode* AnacondaParser::atom()
{
	state_t state = save();

	ExpressionNode *node = paren();
	if (node)
		return node;
	restore(state);

	node = funccall();
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

	ExpressionNode *node = expr();
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

// <funccall> = <WS>? <id> <WS>? '(' <funcargs> <WS>? ')'
FunctionCallNode* AnacondaParser::funccall()
{
	whitespace();
	std::string name = id();
	if (name == "")
		return nullptr;

	whitespace();
	if (!expect('('))
		return nullptr;

	FunctionArguments* args = funcargs();

	whitespace();
	if (!expect(')'))
	{
		delete args;
		return nullptr;
	}

	return new FunctionCallNode(name, args);
}

// <funcargs> = <WS>? '(' (<expr> (',' <expr>)*) <WS>? ')'
FunctionArguments* AnacondaParser::funcargs()
{
	std::vector<ExpressionNode*> arguments;

	whitespace();
	if (!expect('('))
		return nullptr;

	while (true)
	{
		whitespace();
		ExpressionNode *arg = expr();
		if (!arg)
			break;

		arguments.push_back(arg);

		whitespace();
		if (expect(')'))
			return new FunctionArguments(arguments);
		else if (!expect(','))
			break;
	}

	for(auto expr : arguments)
		delete expr;
	return nullptr;
}

// <variable> = <id>
VariableNode* AnacondaParser::variable()
{
	whitespace();
	std::string name = id();
	if (name == "")
		return nullptr;

	return new VariableNode(name);
}
