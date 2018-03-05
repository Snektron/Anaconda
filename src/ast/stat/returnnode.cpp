#include "ast/stat/returnnode.h"
#include "except/exceptions.h"
#include "generator/brainfuck.h"

#include <iostream>
#include <memory>
#include <sstream>

ReturnNode::ReturnNode(ExpressionNode* retval):
    retval(retval) {}

ReturnNode::~ReturnNode()
{
    delete this->retval;
}

void ReturnNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "return statement" << std::endl;
    if(this->retval != nullptr)
        this->retval->print(os, level+1);
}

void ReturnNode::checkTypes(BrainfuckWriter& writer)
{
    if(this->retval != nullptr)
        this->retval->checkTypes(writer);

    size_t current_scope = writer.getScope();
    FunctionDefinition* definition = writer.lookupScope(current_scope);
    std::unique_ptr<DataTypeBase> func_rettype(definition->getReturnType());

    std::unique_ptr<DataTypeBase> return_type(this->retval == nullptr ? new DataType<DataTypeClass::VOID>() : this->retval->getType());

    if(!return_type->equals(*func_rettype.get()))
    {
        std::stringstream ss;
        ss << "Return with expression of type " << *return_type <<
              " in function of type " << *func_rettype << std::endl;
        throw TypeMismatchException(ss.str());
    }
}

void ReturnNode::generate(BrainfuckWriter& writer)
{
    ///TODO
    writer.unimplemented();
}

void ReturnNode::declareLocals(BrainfuckWriter& writer)
{
    this->retval->declareLocals(writer);
}
