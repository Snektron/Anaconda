#ifndef SRC_PARSER_ANACONDAPARSER_H_
#define SRC_PARSER_ANACONDAPARSER_H_

#include "parser/parser.h"
#include "ast/node.h"

class AnacondaParser : public Parser {
public:
	AnacondaParser(const std::string& input);
	StatementListNode* parse();
private:
	std::string name();

	StatementListNode* unit();

	FunctionDeclaration* funcdecl();
	FunctionArgumentDeclarationNode* funcpar();

	StatementListNode* block();

	StatementListNode* statlist();
	Node* statement();
	Node* ifstat();

	ExpressionNode* expr();
	ExpressionNode* sum();
	ExpressionNode* product();
	ExpressionNode* unary();
	ExpressionNode* atom();
	ExpressionNode* paren();
	FunctionCallNode* funccall();
	FunctionArgumentNode* funcargs();
	VariableNode* variable();
};

#endif
