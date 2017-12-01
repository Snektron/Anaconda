#include "generator/brainfuck.h"

BrainfuckWriter::BrainfuckWriter(std::ostream& os) : output(os), current_scope(GLOBAL_SCOPE)
{
    this->scopes.push_back(Scope()); //Create the global scope
}

FunctionDefinition::FunctionDefinition(const std::map<std::string, DataTypeBase*>& arguments, DataTypeBase* return_type, BlockNode* code) : arguments(arguments), return_type(return_type), code(code) {}

FunctionDefinition::FunctionDefinition(FunctionDefinition&& old) : arguments(std::move(old.arguments)), return_type(old.return_type), code(old.code) {}

StructureDefintion::StructureDefintion(const std::map<std::string, DataTypeBase*> fields) : fields(fields) {}

StructureDefinition::StructureDefinition(StructureDefinition&& old) : fields(std::move(old.fields)) {}

size_t BrainfuckWriter::declareFunction(const std::string& name, const std::map<std::string, DataTypeBase*>& arguments, DataTypeBase* return_value, BlockNode* block_node)
{
    this->functions.emplace(name, FunctionDefinition(arguments, return_value, block_node));
    this->scopes.emplace_back(Scope());
    return this->scopes.size() - 1;
}

void BrainfuckWriter::declareVariable(const std::string& name, DataTypeBase* datatype)
{
    Scope& current_scope = this->scopes[this->current_scope];
    current_scope.declareVariable(name, datatype);
}

void BrainfuckWriter::switchScope(size_t new_scope)
{
    this->current_scope = new_scope;
}
