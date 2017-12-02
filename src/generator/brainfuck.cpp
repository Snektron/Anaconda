#include "generator/brainfuck.h"

Scope::Scope(Scope&& old) : declarations(old.declarations), stack_locations(old.stack_locations) {}

void Scope::declareVariable(const std::string& name, DataTypeBase* datatype)
{
    std::map<std::string, DataTypeBase*>& current_scope = this->declarations.back();
    current_scope[name] = datatype;
}

void Scope::setVariableLocation(const std::string& name, size_t location)
{
    std::map<std::string, size_t>& current_scope = this->stack_locations.back();
    current_scope[name] = location;
}

DataTypeBase* Scope::findVariable(const std::string& name)
{
    for(auto it = this->declarations.rbegin(); it != this->declarations.rend(); ++it)
    {
        auto found = it->find(name);
        if(found != it->end())
            return found->second;
    }
    return nullptr;
}

size_t Scope::findVariableLocation(const std::string& name)
{
    for(auto it = this->stack_locations.rbegin(); it != this->stack_locations.rend(); ++it)
    {
        auto found = it->find(name);
        if(found != it->end())
            return found->second;
    }
    return 0;
}

void Scope::enterFrame()
{
    this->declarations.emplace_back(std::map<std::string, DataTypeBase*>());
    this->stack_locations.emplace_back(std::map<std::string, size_t>());
}

void Scope::exitFrame()
{
    this->declarations.pop_back();
    this->stack_locations.pop_back();
}

std::map<std::string, DataTypeBase*>& Scope::getFrameDeclarations()
{
    return this->declarations.back();
}

BrainfuckWriter::BrainfuckWriter(std::ostream& os) : output(os), current_scope(GLOBAL_SCOPE)
{
    this->scopes.push_back(Scope()); //Create the global scope
}

FunctionDefinition::FunctionDefinition(const std::map<std::string, DataTypeBase*>& arguments, DataTypeBase* return_type, BlockNode* code) : arguments(arguments), return_type(return_type), code(code) {}

FunctionDefinition::FunctionDefinition(FunctionDefinition&& old) : arguments(std::move(old.arguments)), return_type(old.return_type), code(old.code) {}

StructureDefinition::StructureDefinition(const std::map<std::string, DataTypeBase*>& fields) : fields(fields) {}

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

void BrainfuckWriter::enterFrame()
{
    Scope& current_scope = this->scopes[this->current_scope];
    current_scope.enterFrame();
}

void BrainfuckWriter::exitFrame()
{
    Scope& current_scope = this->scopes[this->current_scope];
    current_scope.exitFrame();
}
