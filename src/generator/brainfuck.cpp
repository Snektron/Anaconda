#include "generator/brainfuck.h"
#include "except/exceptions.h"

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

bool Scope::hasVariable(const std::string& name)
{
    return this->findVariable(name) != nullptr;
}

bool Scope::hasFrameVariable(const std::string& name)
{
    std::map<std::string, size_t>& current_scope = this->stack_locations.back();
    return current_scope.find(name) != current_scope.end();
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

FunctionDefinition::FunctionDefinition(const std::vector<Field>& arguments, DataTypeBase* return_type, BlockNode* code):
	arguments(arguments), return_type(return_type), code(code) {}

FunctionDefinition::FunctionDefinition(FunctionDefinition&& old):
	arguments(std::move(old.arguments)), return_type(old.return_type), code(old.code) {}

bool FunctionDefinition::parametersEqual(const std::vector<DataTypeBase*>& arguments)
{
    if(arguments.size() != this->arguments.size())
        return false;
    for(size_t i = 0; i < this->arguments.size(); ++i)
    {
        if(!this->arguments[i].type()->equals(*arguments[i]))
            return false;
    }
    return true;
}

StructureDefinition::StructureDefinition(const std::vector<Field>& fields):
	fields(fields) {}

StructureDefinition::StructureDefinition(StructureDefinition&& old) : fields(std::move(old.fields)) {}

BrainfuckWriter::BrainfuckWriter(std::ostream& os) : output(os), current_scope(GLOBAL_SCOPE)
{
    //Create the global scope, which always has exactly one frame
    Scope global_scope;
    global_scope.enterFrame();
    this->scopes.emplace_back(std::move(global_scope));
}

size_t BrainfuckWriter::declareFunction(const std::string& name, const std::vector<Field>& arguments, DataTypeBase* return_value, BlockNode* block_node)
{
    if(this->isFunctionDeclared(name, arguments))
        throw RedefinitionException("Redefinition of function " + name);
    this->functions.emplace(name, FunctionDefinition(arguments, return_value, block_node));
    this->scopes.emplace_back(Scope());
    return this->scopes.size() - 1;
}

void BrainfuckWriter::declareStructure(const std::string& name, const std::vector<Field>& members)
{
    if(this->isStructureDeclared(name))
        throw RedefinitionException("Redefinition of structure " + name);
    this->structures.emplace(name, StructureDefinition(members));
}

void BrainfuckWriter::declareVariable(const std::string& name, DataTypeBase* datatype)
{
    Scope& current_scope = this->scopes[this->current_scope];
    if(current_scope.hasFrameVariable(name))
        throw RedefinitionException("Redefinition of variable " + name);
    current_scope.declareVariable(name, datatype);
}

bool BrainfuckWriter::isFunctionDeclared(const std::string& name, const std::vector<Field>& arguments)
{
    std::vector<const DataTypeBase*> argument_types;
    for(auto& it : arguments)
        argument_types.push_back(it.type());
    return this->isFunctionDeclared(name, arguments);
}

bool BrainfuckWriter::isFunctionDeclared(const std::string& name, const std::vector<DataTypeBase*>& arguments)
{
    auto equal_range = this->functions.equal_range(name);
    
    for(auto& it = equal_range.first; it != equal_range.second; ++it)
    {
        if(it->second.parametersEqual(arguments))
            return true;
    }
    return false;
}

bool BrainfuckWriter::isStructureDeclared(const std::string& name)
{
    return this->structures.find(name) != this->structures.end();
}

void BrainfuckWriter::switchScope(size_t new_scope)
{
    this->current_scope = new_scope;
}

size_t BrainfuckWriter::getScope()
{
    return this->current_scope;
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
