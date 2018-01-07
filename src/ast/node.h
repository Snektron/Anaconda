#ifndef NODE_HPP_INCLUDED
#define NODE_HPP_INCLUDED

#include <iosfwd>
#include <string>
#include <vector>
#include <map>
#include "types/datatype.h"
#include "common/field.h"

/*
 * Node type:
 *
 * Function Declaration
 *
 * Global Namespace
 * Statement List
 * Block
 * Global declaration
 * Structure type definition
 *
 * Empty Statement
 * Expression Statement
 * Declaration Statement
 * Assignment Statement
 * While Statement
 * If Statement
 * If Else Statement
 * Assembly Statement
 * Return Statement
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
class FieldListNode;
class ArgumentListNode;
class GlobalElementNode;
class VariableNode;

class Node
{
    protected:
        void printIndent(std::ostream&, size_t) const;
    public:
        virtual ~Node() = default;

        virtual void print(std::ostream&, size_t) const = 0;
        virtual void generate(BrainfuckWriter&) = 0;
        virtual void declareGlobals(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&) = 0;
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
        virtual void declareGlobals(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
};

class GlobalElementNode : public Node
{
    public:
        virtual ~GlobalElementNode() = default;
};

class GlobalExpressionNode : public GlobalElementNode
{
    private:
        ExpressionNode* expression;
    public:
        GlobalExpressionNode(ExpressionNode*);
        virtual ~GlobalExpressionNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareGlobals(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
};

class FunctionDeclaration : public GlobalElementNode
{
    private:
        std::string name;
        FieldListNode* parameters;
        DataTypeBase* return_type;
        BlockNode* content;
        size_t scope;
    public:
        FunctionDeclaration(const std::string&, FieldListNode*, DataTypeBase*, BlockNode*);
        virtual ~FunctionDeclaration();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareGlobals(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
};

class FieldListNode : public Node
{
    private:
        std::vector<Field> arguments;
    public:
        FieldListNode(const std::vector<Field>&);
        virtual ~FieldListNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);

        std::vector<Field>& getParameters();
};

class StructureDefinitionNode : public GlobalElementNode
{
    private:
        std::string name;
        FieldListNode* members;
        DataType<DataTypeClass::STRUCT_FORWARD>* type;
    public:
        StructureDefinitionNode(const std::string&, FieldListNode* members);
        ~StructureDefinitionNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareGlobals(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
};

class StatementNode : public Node
{
    public:
        virtual ~StatementNode() = default;

        virtual void declareLocals(BrainfuckWriter&) = 0;
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
        virtual void checkTypes(BrainfuckWriter&);
        virtual void declareLocals(BrainfuckWriter&);
};

class EmptyStatementNode : public StatementNode
{
    public:
        EmptyStatementNode();
        virtual ~EmptyStatementNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual void declareLocals(BrainfuckWriter&);
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
        virtual void checkTypes(BrainfuckWriter&);
        virtual void declareLocals(BrainfuckWriter&);
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
        virtual void checkTypes(BrainfuckWriter&);
        virtual void declareLocals(BrainfuckWriter&);
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
        virtual void checkTypes(BrainfuckWriter&);
        virtual void declareLocals(BrainfuckWriter&);
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
        virtual void checkTypes(BrainfuckWriter&);
        virtual void declareLocals(BrainfuckWriter&);
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
        virtual void checkTypes(BrainfuckWriter&);
        virtual void declareLocals(BrainfuckWriter&);
};

class ReturnNode : public StatementNode
{
    private:
        ExpressionNode* retval;
    public:
        ReturnNode(ExpressionNode*);
        virtual ~ReturnNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual void declareLocals(BrainfuckWriter&);
};

class ExpressionNode : public Node
{
    public:
        virtual ~ExpressionNode() = default;

        virtual DataTypeBase* getType() = 0;
        virtual void declareLocals(BrainfuckWriter&) = 0;
};

class UnaryExpressionNode : public ExpressionNode
{
    protected:
        ExpressionNode* op;
        DataTypeBase* type;

        UnaryExpressionNode(ExpressionNode*);
    public:
        virtual ~UnaryExpressionNode();

        virtual void checkTypes(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);
};

class BinaryExpressionNode : public ExpressionNode
{
    protected:
        ExpressionNode* lop;
        ExpressionNode* rop;
        DataTypeBase* type;

        BinaryExpressionNode(ExpressionNode*, ExpressionNode*);
    public:
        virtual ~BinaryExpressionNode();

        virtual void checkTypes(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);
};

class AssignmentNode : public ExpressionNode
{
    private:
        ExpressionNode* lop;
        ExpressionNode* rop;
    public:
        AssignmentNode(ExpressionNode*, ExpressionNode*);
        virtual ~AssignmentNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);
};

class AddNode : public BinaryExpressionNode
{
    public:
        AddNode(ExpressionNode*, ExpressionNode*);
        virtual ~AddNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

class SubNode : public BinaryExpressionNode
{
    public:
        SubNode(ExpressionNode*, ExpressionNode*);
        virtual ~SubNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

class MulNode : public BinaryExpressionNode
{
    public:
        MulNode(ExpressionNode*, ExpressionNode*);
        virtual ~MulNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

class DivNode : public BinaryExpressionNode
{
    public:
        DivNode(ExpressionNode*, ExpressionNode*);
        virtual ~DivNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

class ModNode : public BinaryExpressionNode
{
    public:
        ModNode(ExpressionNode*, ExpressionNode*);
        virtual ~ModNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

class NegateNode : public UnaryExpressionNode
{
    public:
        NegateNode(ExpressionNode*);
        virtual ~NegateNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

class ComplementNode : public UnaryExpressionNode
{
    public:
        ComplementNode(ExpressionNode*);
        virtual ~ComplementNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

class BitwiseAndNode : public BinaryExpressionNode
{
    public:
        BitwiseAndNode(ExpressionNode*, ExpressionNode*);
        virtual ~BitwiseAndNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

class BitwiseOrNode : public BinaryExpressionNode
{
    public:
        BitwiseOrNode(ExpressionNode*, ExpressionNode*);
        virtual ~BitwiseOrNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

class BitwiseXorNode : public BinaryExpressionNode
{
    public:
        BitwiseXorNode(ExpressionNode*, ExpressionNode*);
        virtual ~BitwiseXorNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

class BitwiseLeftShiftNode : public BinaryExpressionNode
{
    public:
        BitwiseLeftShiftNode(ExpressionNode*, ExpressionNode*);
        virtual ~BitwiseLeftShiftNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
};

class BitwiseRightShiftNode : public BinaryExpressionNode
{
    public:
        BitwiseRightShiftNode(ExpressionNode*, ExpressionNode*);
        virtual ~BitwiseRightShiftNode() = default;

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
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
        virtual void checkTypes(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);
};

class DeclarationNode: public ExpressionNode
{
    private:
        DataTypeBase* datatype;
        std::string variable;
    public:
        DeclarationNode(DataTypeBase*, const std::string&);
        virtual ~DeclarationNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual void declareGlobals(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);
};

class VariableNode : public ExpressionNode
{
    private:
        std::string variable;
        DataTypeBase* datatype;
    public:
        VariableNode(const std::string&);
        virtual ~VariableNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);

        std::string getName();
};

class FunctionCallNode : public ExpressionNode
{
    private:
        std::string function_var;
        ArgumentListNode* arguments;
        DataTypeBase* called_type;
    public:
        FunctionCallNode(const std::string&, ArgumentListNode*);
        virtual ~FunctionCallNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);
};

class ArgumentListNode : public Node
{
    private:
        std::vector<ExpressionNode*> arguments;
    public:
        ArgumentListNode(const std::vector<ExpressionNode*>& arguments);
        virtual ~ArgumentListNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void declareGlobals(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);

        std::vector<DataTypeBase*> getArgumentTypes();
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
        virtual void checkTypes(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);
};

class AssemblyNode : public ExpressionNode
{
    private:
        DataTypeBase* datatype;
        std::string assembly;
        ArgumentListNode* arguments;
    public:
        AssemblyNode(DataTypeBase*, const std::string&, ArgumentListNode*);
        virtual ~AssemblyNode();

        virtual void print(std::ostream&, size_t) const;
        virtual void generate(BrainfuckWriter&);
        virtual void checkTypes(BrainfuckWriter&);
        virtual DataTypeBase* getType();
        virtual void declareLocals(BrainfuckWriter&);
};

#endif
