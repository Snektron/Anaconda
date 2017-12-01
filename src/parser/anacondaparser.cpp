#include "parser/anacondaparser.h"

AnacondaParser::AnacondaParser(const std::string& input):
	Parser(input) {}

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

StatementListNode* AnacondaParser::block() {
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

StatementListNode* AnacondaParser::statlist() {
	return nullptr;
}

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
