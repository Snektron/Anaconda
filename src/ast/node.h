#ifndef NODE_HPP_INCLUDED
#define NODE_HPP_INCLUDED

#include <iosfwd>
#include <string>
#include <vector>
#include <map>

/*
 * Node type:
 * 
 * Function Declaration
 *
 * Global Namespace
 * Statement List
 * Block
 *
 * Expression Statement
 * Declaration Statement
 * While Statement
 * If Statement
 * If Else Statement
 * Assembly Statement
 *
 * Expression
 * Add Expression
 * Sub Expression
 * Mul Expression
 * Div Expression
 * Mod Expression
 * Negate Expression
 * Bitwise Complement Expression
 * Bitwise And Expression
 * Bitwise Or Expression
 * Bitwise Xor Expression
 * Bitwise Left Shift Expression
 * Bitwise Right Shift Expression
 * Constant Expression
 * Variable Expression
 * Function Call Expression
 * Cast Expression
 */

class BrainfuckWriter;
class DataTypeBase;

class StatementNode;
class ExpressionNode;
class BlockNode;
class FunctionParameters;
class FunctionArguments;
class GlobalElementNode;

class Node
{
    protected:
        void printIndent(std::ostream&, size_t) const;
	public:
		virtual ~Node() = default;
		
		virtual void print(std::ostream&, size_t) const = 0;
		virtual void generate(BrainfuckWriter&) = 0;
        virtual void declareTypes(BrainfuckWriter&, std::ostream&) = 0;
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&) = 0;
};

class GlobalNode : public Node
{
    private:
        std::vector<GlobalElementNode*> elements;
    public:
        GlobalNode(const std::vector<GlobalElementNode*>&);
        virtual ~GlobalNode();
        
        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class GlobalElementNode : public Node
{
    public:
        virtual ~GlobalElementNode() = default;
};

class GlobalDeclarationNode : public GlobalElementNode
{
	private:
		DataTypeBase* variable_type;
		std::string variable_name;
		ExpressionNode* initializer;
	public:
		GlobalDeclarationNode(DataTypeBase*, const std::string&);
		GlobalDeclarationNode(DataTypeBase*, const std::string&, ExpressionNode*);
		virtual ~GlobalDeclarationNode();
	
		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class StatementNode : public Node
{
    public:
	    virtual ~StatementNode() = default;
};

class FunctionDeclaration : public GlobalElementNode
{
	private:
		std::string name;
        FunctionParameters* parameters;
		DataTypeBase* return_type;
        BlockNode* content;
	public:
		FunctionDeclaration(const std::string&, FunctionParameters*, DataTypeBase*, BlockNode*);
		virtual ~FunctionDeclaration();
		
		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class FunctionParameters : public Node
{
	private:
		std::map<std::string, DataTypeBase*> arguments;
	public:
		FunctionParameters(const std::map<std::string, DataTypeBase*>&);
		virtual ~FunctionParameters();

		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class StatementListNode : public StatementNode
{
	private:
		StatementNode* first;
		StatementNode* second;
	public:
		StatementListNode(StatementNode*, StatementNode*);
		virtual ~StatementListNode();

		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class BlockNode : public StatementNode
{
	private:
		StatementNode* content;
	public:
		BlockNode(StatementNode*);
		virtual ~BlockNode();
	
		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class ExpressionStatementNode : public StatementNode
{
	private:
		ExpressionNode* content;
	public:
		ExpressionStatementNode(ExpressionNode*);
		virtual ~ExpressionStatementNode();
	
		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class DeclarationNode : public StatementNode
{
	private:
		DataTypeBase* variable_type;
		std::string variable_name;
		ExpressionNode* initializer;
	public:
		DeclarationNode(DataTypeBase*, const std::string&);
		DeclarationNode(DataTypeBase*, const std::string&, ExpressionNode*);
		virtual ~DeclarationNode();
	
		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class AssignmentNode : public StatementNode
{
    private:
        std::string variable_name;
        ExpressionNode* expression;
    public:
        AssignmentNode(const std::string&, ExpressionNode*);
        virtual ~AssignmentNode();
        
        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class WhileNode : public StatementNode
{
	private:
		ExpressionNode* conditional;
		StatementNode* statement;
	public:
		WhileNode(ExpressionNode*, StatementNode*);
		virtual ~WhileNode();
		
		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class IfNode : public StatementNode
{
	private:
		ExpressionNode* conditional;
		StatementNode* statement;
	public:
		IfNode(ExpressionNode*, StatementNode*);
		virtual ~IfNode();
		
		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
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
		
		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class AssemblyNode : public StatementNode
{
    private:
        std::string assembly;
    public:
        AssemblyNode(const std::string&);
        virtual ~AssemblyNode();
        
        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
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
		
		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class SubNode : public ExpressionNode
{
	private:
		ExpressionNode* lop;
		ExpressionNode* rop;
	public:
		SubNode(ExpressionNode*, ExpressionNode*);
		virtual ~SubNode();
		
		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class MulNode : public ExpressionNode
{
	private:
		ExpressionNode* lop;
		ExpressionNode* rop;
	public:
		MulNode(ExpressionNode*, ExpressionNode*);
		virtual ~MulNode();
		
		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class DivNode : public ExpressionNode
{
	private:
		ExpressionNode* lop;
		ExpressionNode* rop;
	public:
		DivNode(ExpressionNode*, ExpressionNode*);
		virtual ~DivNode();
		
		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class ModNode : public ExpressionNode
{
	private:
		ExpressionNode* lop;
		ExpressionNode* rop;
	public:
		ModNode(ExpressionNode*, ExpressionNode*);
		virtual ~ModNode();
		
		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class NegateNode : public ExpressionNode
{
    private:
        ExpressionNode* op;
    public:
        NegateNode(ExpressionNode*);
        virtual ~NegateNode();
        
        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class ComplementNode : public ExpressionNode
{
    private:
        ExpressionNode* op;
    public:
        ComplementNode(ExpressionNode*);
        virtual ~ComplementNode();
        
        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class BitwiseAndNode : public ExpressionNode
{
    private:
        ExpressionNode* lop;
        ExpressionNode* rop;
    public:
        BitwiseAndNode(ExpressionNode*, ExpressionNode*);
        virtual ~BitwiseAndNode();
        
        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class BitwiseOrNode : public ExpressionNode
{
    private:
        ExpressionNode* lop;
        ExpressionNode* rop;
    public:
        BitwiseOrNode(ExpressionNode*, ExpressionNode*);
        virtual ~BitwiseOrNode();
        
        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class BitwiseXorNode : public ExpressionNode
{
    private:
        ExpressionNode* lop;
        ExpressionNode* rop;
    public:
        BitwiseXorNode(ExpressionNode*, ExpressionNode*);
        virtual ~BitwiseXorNode();
        
        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class BitwiseLeftShiftNode : public ExpressionNode
{
    private:
        ExpressionNode* lop;
        ExpressionNode* rop;
    public:
        BitwiseLeftShiftNode(ExpressionNode*, ExpressionNode*);
        virtual ~BitwiseLeftShiftNode();
        
        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class BitwiseRightShiftNode : public ExpressionNode
{
    private:
        ExpressionNode* lop;
        ExpressionNode* rop;
    public:
        BitwiseRightShiftNode(ExpressionNode*, ExpressionNode*);
        virtual ~BitwiseRightShiftNode();
        
        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class U8ConstantNode : public ExpressionNode
{
	private:
		uint8_t value;
	public:
		U8ConstantNode(uint8_t);
		virtual ~U8ConstantNode();

		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class VariableNode : public ExpressionNode
{
	private:
		std::string variable;
	public:
		VariableNode(const std::string&);
		virtual ~VariableNode();
		
		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class FunctionCallNode : public ExpressionNode
{
	private:
		std::string function_name;
		FunctionArguments* arguments;
	public:
		FunctionCallNode(const std::string&, FunctionArguments* arguments);
		virtual ~FunctionCallNode();
		
		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class FunctionArguments : public Node
{
	private:
		std::vector<ExpressionNode*> arguments;
	public:
		FunctionArguments(const std::vector<ExpressionNode*>& arguments);
		virtual ~FunctionArguments();

		virtual void print(std::ostream&, size_t) const;
		virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

class CastExpression : public ExpressionNode
{
    private:
        ExpressionNode* expression;
        DataTypeBase* desired_type;
    public:
        CastExpression(ExpressionNode*, DataTypeBase*);
        virtual ~CastExpression();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareTypes(BrainfuckWriter&, std::ostream&);
        virtual bool checkTypes(BrainfuckWriter&, std::ostream&);
};

#endif
