#include "ast/expr/functioncallnode.h"
#include "generator/brainfuck.h"
#include "except/exceptions.h"
#include "common/util.h"

#include <iostream>
#include <sstream>

FunctionCallNode::FunctionCallNode(const std::string& function_var, ArgumentListNode* arguments):
    function_var(function_var), arguments(arguments), called_type(nullptr) {}

FunctionCallNode::~FunctionCallNode()
{
    delete this->arguments;
    delete this->called_type;
}

void FunctionCallNode::print(std::ostream& os, size_t level) const
{
    this->printIndent(os, level);
    os << "function call (" << this->function_var << ")" << std::endl;
    this->arguments->print(os, level+1);
}

void FunctionCallNode::generate(BrainfuckWriter& writer)
{
    ///TODO
    writer.unimplemented();
}

void FunctionCallNode::checkTypes(BrainfuckWriter& writer)
{
    this->arguments->checkTypes(writer);

    std::vector<DataTypeBase*> arg_types = this->arguments->getArgumentTypes();
    try
    {
        FunctionDefinition* definition = writer.getDeclaredFunction(this->function_var, arg_types);
        if(definition == nullptr)
        {
            std::stringstream ss;
            ss << "Attempt to call undeclared function " << this->function_var << "(";
            bool first = true;
            for(DataTypeBase* dtype : arg_types)
            {
                if(!first)
                    ss << ", ";
                else
                    first = false;
                ss << *dtype;
            }
            ss << ")";
            throw TypeCheckException(ss.str());
        }
        this->called_type = definition->getReturnType();

        for(DataTypeBase* dtype : arg_types)
            delete dtype;
    }
    catch(...)
    {
        for(DataTypeBase* dtype : arg_types)
            delete dtype;
        throw;
    }
}

DataTypeBase* FunctionCallNode::getType()
{
    return this->called_type;
}

void FunctionCallNode::declareLocals(BrainfuckWriter& writer)
{
    UNUSED(writer);
}
