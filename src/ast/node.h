#ifndef NODE_HPP_INCLUDED
#define NODE_HPP_INCLUDED

#include <iosfwd>
#include <string>

/*
 * Node type:
 * 
 * Function Declaration
 *
 * Statement List
 * Block
 *
 * Expression Statement
 * Assignment Statement
 * While Statement
 * If Statement
 * If Else Statement
 *
 * Expression
 * Add Expression
 * Sub Expression
 * Mul Expression
 * Div Expression
 * Mod Expression
 * Constant Expression
 * Variable Expression
 * Function Call Expression
 */

enum DataType
{
	T_VOID,
	T_U8
};

class StatementNode;
class ExpressionNode;
class FunctionArgumentDeclarationNode;
class FunctionArgumentNode;

class Node
{
	public:
		virtual ~Node() = default;
		
		virtual void print(std::ostream&) = 0;
		virtual void generate(std::ostream&) = 0;
};

class StatementNode : public Node
{
	virtual ~StatementNode() = default;
};

class FunctionDeclaration : public Node
{
	private:
		std::string name;
		FunctionArgumentDeclarationNode* args;
		DataType return_type;
	public:
		FunctionDeclaration(const std::string&, FunctionArgumentDeclarationNode*, DataType);
		virtual ~FunctionDeclaration();
		
		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

class FunctionArgumentDeclarationNode : public Node
{
	private:
		std::string name;
		DataType arg_type;
		FunctionArgumentDeclarationNode* next;
	public:
		FunctionArgumentDeclarationNode(const std::string&, DataType);
		FunctionArgumentDeclarationNode(const std::string&, DataType, FunctionArgumentDeclarationNode*);
		virtual ~FunctionArgumentDeclarationNode();

		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

class StatementListNode : public StatementNode
{
	private:
		StatementNode* first;
		StatementNode* second;
	public:
		StatementListNode(StatementNode*, StatementNode*);
		virtual ~StatementListNode();

		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

class BlockNode : public StatementNode
{
	private:
		StatementNode* content;
	public:
		BlockNode(StatementNode*);
		virtual ~BlockNode();
	
		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

class ExpressionStatementNode : public StatementNode
{
	private:
		ExpressionNode* content;
	public:
		ExpressionStatementNode(ExpressionNode*);
		virtual ~ExpressionStatementNode();
	
		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

class AssignmentNode : public StatementNode
{
	private:
		DataType variable_type;
		std::string variable_name;
		ExpressionNode* initializer;
	public:
		AssignmentNode(DataType, const std::string&);
		AssignmentNode(DataType, const std::string&, ExpressionNode*);
		virtual ~AssignmentNode();
	
		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

class WhileNode : public StatementNode
{
	private:
		ExpressionNode* conditional;
		StatementNode* statement;
	public:
		WhileNode(ExpressionNode*, StatementNode*);
		virtual ~WhileNode();
		
		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

class IfNode : public StatementNode
{
	private:
		ExpressionNode* conditional;
		StatementNode* statement;
	public:
		IfNode(ExpressionNode*, StatementNode*);
		virtual ~IfNode();
		
		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

class IfElseNode : public StatementNode
{
	private:
		ExpressionNode* conditional;
		StatementNode* statement;
		StatementNode* else_statement;
	public:
		IfElseNode(ExpressionNode*, StatementNode*, StatementNode*);
		virtual ~IfElseNode();
		
		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

class ExpressionNode : public Node
{
	public:
		virtual ~ExpressionNode() = default;
};

class AddNode : public ExpressionNode
{
	private:
		ExpressionNode* lop;
		ExpressionNode* rop;
	public:
		AddNode(ExpressionNode*, ExpressionNode*);
		virtual ~AddNode();
		
		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

class SubNode : public ExpressionNode
{
	private:
		ExpressionNode* lop;
		ExpressionNode* rop;
	public:
		SubNode(ExpressionNode*, ExpressionNode*);
		virtual ~SubNode();
		
		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

class MulNode : public ExpressionNode
{
	private:
		ExpressionNode* lop;
		ExpressionNode* rop;
	public:
		MulNode(ExpressionNode*, ExpressionNode*);
		virtual ~MulNode();
		
		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

class DivNode : public ExpressionNode
{
	private:
		ExpressionNode* lop;
		ExpressionNode* rop;
	public:
		DivNode(ExpressionNode*, ExpressionNode*);
		virtual ~DivNode();
		
		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

class ModNode : public ExpressionNode
{
	private:
		ExpressionNode* lop;
		ExpressionNode* rop;
	public:
		ModNode(ExpressionNode*, ExpressionNode*);
		virtual ~ModNode();
		
		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

class U8ConstantNode : public ExpressionNode
{
	private:
		uint8_t value;
	public:
		U8ConstantNode(uint8_t);
		virtual ~U8ConstantNode();

		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

class VariableNode : public ExpressionNode
{
	private:
		std::string variable;
	public:
		VariableNode(const std::string&);
		virtual ~VariableNode();
		
		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

class FunctionCallNode : public ExpressionNode
{
	private:
		std::string function_name;
		FunctionArgumentNode* arguments;
	public:
		FunctionCallNode(const std::string&, FunctionArgumentNode*);
		virtual ~FunctionCallNode();
		
		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

class FunctionArgumentNode : public Node
{
	private:
		ExpressionNode* argument;
		FunctionArgumentNode* next;
	public:
		FunctionArgumentNode(ExpressionNode*);
		FunctionArgumentNode(ExpressionNode*, FunctionArgumentNode*);
		virtual ~FunctionArgumentNode();

		virtual void print(std::ostream&);
		virtual void generate(std::ostream&);
};

#endif
