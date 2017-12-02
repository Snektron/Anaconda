#include "ast/node.h"
#include "types/datatype.h"

#include <iostream>

FunctionDeclaration::FunctionDeclaration(const std::string& name, FunctionParameters* parameters, DataTypeBase* return_type, BlockNode* content) : name(name), parameters(parameters), return_type(return_type), content(content) {}

FunctionDeclaration::~FunctionDeclaration()
{
    delete this->content;
    delete this->return_type;
    delete this->parameters;
}

void FunctionDeclaration::print(std::ostream& output, size_t level) const
{
    this->printIndent(output, level);
    output << "function declaration (" << this->name << ")" << std::endl;
    this->printIndent(output, level+1);
    output << "return type: " << *this->return_type << std::endl;
    this->parameters->print(output, level+1);
}
