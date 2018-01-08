#include "generator/brainfuck.h"
#include "except/exceptions.h"

#include <memory>

Scope::Scope(Scope&& old):
    declarations(old.declarations), stack_locations(old.stack_locations)
{
}

void Scope::declareVariable(const std::string& name, const DataTypeBase* datatype)
{
    std::map<std::string, const DataTypeBase*>& current_scope = this->declarations.back();
    current_scope[name] = datatype;
}

void Scope::setVariableLocation(const std::string& name, size_t location)
{
    std::map<std::string, size_t>& current_scope = this->stack_locations.back();
    current_scope[name] = location;
}

const DataTypeBase* Scope::findVariable(const std::string& name)
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
    this->declarations.emplace_back(std::map<std::string, const DataTypeBase*>());
    this->stack_locations.emplace_back(std::map<std::string, size_t>());
}

void Scope::exitFrame()
{
    this->declarations.pop_back();
    this->stack_locations.pop_back();
}

std::map<std::string, const DataTypeBase*>& Scope::getFrameDeclarations()
{
    return this->declarations.back();
}

FunctionDefinition::FunctionDefinition(const std::vector<Field>& arguments, const DataTypeBase* return_type, BlockNode* code):
    arguments(arguments), return_type(return_type), code(code) {}

FunctionDefinition::FunctionDefinition(FunctionDefinition&& old):
    arguments(std::move(old.arguments)), return_type(old.return_type), code(old.code) {}

bool FunctionDefinition::parametersEqual(const std::vector<DataTypeBase*>& arguments)
{
    if(arguments.size() != this->arguments.size())
        return false;
    for(size_t i = 0; i < this->arguments.size(); ++i)
    {
        if(!this->arguments[i].getType()->equals(*arguments[i]))
            return false;
    }
    return true;
}

DataTypeBase* FunctionDefinition::getReturnType() const
{
    return this->return_type->copy();
}

StructureDefinition::StructureDefinition(const std::vector<Field>& fields):
    fields(fields) {}

StructureDefinition::StructureDefinition(StructureDefinition&& old):
    fields(std::move(old.fields)) {}

size_t StructureDefinition::size(BrainfuckWriter& writer) const
{
    size_t total_size = 0;
    for(const Field& field : this->fields)
    {
        total_size += field.getType()->size(writer);
    }
    return total_size;
}

VariableDefinition::VariableDefinition(const DataTypeBase* datatype, size_t location) : datatype(datatype), stack_location(location) {}

DataTypeBase* VariableDefinition::dataType() const
{
    return this->datatype->copy();
}

size_t VariableDefinition::location() const
{
    return this->stack_location;
}

BrainfuckWriter::BrainfuckWriter(std::ostream& os):
    output(&os), current_scope(GLOBAL_SCOPE), stack_pointer(0)
{
    //Create the global scope, which always has exactly one frame
    Scope global_scope;
    global_scope.enterFrame();
    this->scopes.emplace_back(std::move(global_scope));
    this->scope_func_lookup.emplace_back(nullptr);
}

size_t BrainfuckWriter::declareFunction(const std::string& name, const std::vector<Field>& arguments, const DataTypeBase* return_value, BlockNode* block_node)
{
    if(this->isFunctionDeclared(name, arguments))
        throw RedefinitionException("Redefinition of function " + name);
    auto it = this->functions.emplace(name, FunctionDefinition(arguments, return_value, block_node));
    this->scopes.emplace_back(Scope());
    this->scope_func_lookup.emplace_back(&it->second);
    return this->scopes.size() - 1;
}

void BrainfuckWriter::declareStructure(const std::string& name, const std::vector<Field>& members)
{
    if(this->isStructureDeclared(name))
        throw RedefinitionException("Redefinition of structure " + name);
    this->structures.emplace(name, StructureDefinition(members));
}

void BrainfuckWriter::declareVariable(const std::string& name, const DataTypeBase* datatype)
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
        argument_types.push_back(it.getType());
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

FunctionDefinition* BrainfuckWriter::getDeclaredFunction(const std::string& name, const std::vector<DataTypeBase*>& arguments)
{
    auto equal_range = this->functions.equal_range(name);

    for(auto& it = equal_range.first; it != equal_range.second; ++it)
    {
        if(it->second.parametersEqual(arguments))
            return &it->second;
    }
    return nullptr;
}

StructureDefinition* BrainfuckWriter::getDeclaredStructure(const std::string& name)
{
    auto it = this->structures.find(name);
    if(it != this->structures.end())
        return &it->second;
    return nullptr;
}

VariableDefinition* BrainfuckWriter::getDeclaredVariable(const std::string& variable)
{
    const DataTypeBase* datatype = this->scopes[this->current_scope].findVariable(variable);
    size_t location = this->scopes[this->current_scope].findVariableLocation(variable);
    if(datatype == nullptr)
    {
        datatype = this->scopes[GLOBAL_SCOPE].findVariable(variable);
        location = this->scopes[GLOBAL_SCOPE].findVariableLocation(variable);
    }

    if(datatype == nullptr)
        return nullptr;
    return new VariableDefinition(datatype, location);
}

void BrainfuckWriter::switchScope(size_t new_scope)
{
    this->current_scope = new_scope;
}

size_t BrainfuckWriter::getScope()
{
    return this->current_scope;
}

FunctionDefinition* BrainfuckWriter::lookupScope(size_t index)
{
    return this->scope_func_lookup[index];
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

std::ostream& BrainfuckWriter::getOutput()
{
    return *this->output;
}

std::ostream& BrainfuckWriter::setOutput(std::ostream& output)
{
    std::ostream* result = this->output;
    this->output = &output;
    return *result;
}

void BrainfuckWriter::copyAssembly(const std::string& code)
{
    std::ostream& output = this->getOutput();
    for(char c : code)
    {
        //Keep track of the branch operations
        if(c == '[')
            this->branchOpen();
        else if(c == '>')
            this->branchClose();
        else
            output << c;
    }
}

void BrainfuckWriter::increment()
{
    this->getOutput() << "+";
}

void BrainfuckWriter::decrement()
{
    this->getOutput() << "-";
}

void BrainfuckWriter::incrementBy(size_t num)
{
    for(size_t i = 0; i < num; ++i)
        this->increment();
}

void BrainfuckWriter::decrementBy(size_t num)
{
    for(size_t i = 0; i < num; ++i)
        this->decrement();
}

void BrainfuckWriter::incrementStackPointer()
{
    this->getOutput() << ">";
    ++this->stack_pointer;
}

void BrainfuckWriter::decrementStackPointer()
{
    this->getOutput() << "<";
    --this->stack_pointer;
}

void BrainfuckWriter::branchOpen()
{
    this->getOutput() << "[";
}

void BrainfuckWriter::branchClose()
{
    this->getOutput() << "]";
}

void BrainfuckWriter::incrementStackPointerBy(size_t num)
{
    for(size_t i = 0; i < num; ++i)
        this->incrementStackPointer();
}

void BrainfuckWriter::decrementStackPointerBy(size_t num)
{
    for(size_t i = 0; i < num; ++i)
        this->decrementStackPointer();
}

void BrainfuckWriter::moveStackPointerTo(size_t index)
{
    if(index < this->stack_pointer)
        this->decrementStackPointerBy(this->stack_pointer - index);
    else
        this->incrementStackPointerBy(index - this->stack_pointer);
}

void BrainfuckWriter::makeStackFrame()
{
    std::map<std::string, const DataTypeBase*>& variables = this->scopes[this->current_scope].getFrameDeclarations();
    for(auto& it : variables)
    {
        this->scopes[this->current_scope].setVariableLocation(it.first, this->stack_pointer);
        this->push(it.second);
    }
}

void BrainfuckWriter::destroyStackFrame()
{
    std::map<std::string, const DataTypeBase*>& variables = this->scopes[this->current_scope].getFrameDeclarations();
    for(auto& it : variables)
    {
        this->pop(it.second);
    }
}

void BrainfuckWriter::push(const DataTypeBase* datatype)
{
    this->incrementStackPointerBy(datatype->size(*this));
}

void BrainfuckWriter::pop(const DataTypeBase* datatype)
{
    this->decrementStackPointerBy(datatype->size(*this));
}

void BrainfuckWriter::pushByte(uint8_t value)
{
    this->clearByte();
    this->incrementBy(value);
}

void BrainfuckWriter::clearByte()
{
    this->branchOpen();
    this->decrement();
    this->branchClose();
}

void BrainfuckWriter::copyByte(size_t from, size_t to, size_t temp)
{
    size_t old_stack_pointer = this->stack_pointer;

    this->moveStackPointerTo(temp);
    this->clearByte();

    this->moveStackPointerTo(to);
    this->clearByte();

    this->moveStackPointerTo(from);
    this->branchOpen();
    this->moveStackPointerTo(to);
    this->increment();
    this->moveStackPointerTo(temp);
    this->increment();
    this->moveStackPointerTo(from);
    this->decrement();
    this->branchClose();

    this->moveStackPointerTo(temp);
    this->branchOpen();
    this->moveStackPointerTo(from);
    this->increment();
    this->moveStackPointerTo(temp);
    this->decrement();
    this->branchClose();

    //Restore stack pointer
    this->moveStackPointerTo(old_stack_pointer);
}

void BrainfuckWriter::copyValue(size_t from, size_t to, size_t temp, size_t size)
{
    for(size_t i = 0; i < size; ++i)
        this->copyByte(from + i, to + i, temp + i);
}

void BrainfuckWriter::loadValue(size_t from, size_t size)
{
    size_t variable_start = this->stack_pointer;
    this->incrementStackPointerBy(size);
    size_t temporary_start = this->stack_pointer;
    this->incrementStackPointerBy(size);
    this->copyValue(from, variable_start, temporary_start, size);

    //Clean up temporary storage
    this->decrementStackPointerBy(size);
}

void BrainfuckWriter::addU8()
{
    //Assume stack top contains 2 u8
    size_t x = this->stack_pointer - 2;
    size_t y = this->stack_pointer - 1;
    size_t temp = this->stack_pointer;
    //Create temporary storage
    this->incrementStackPointer();

    //Add routine
    this->moveStackPointerTo(temp);
    this->clearByte();

    this->moveStackPointerTo(y);
    this->branchOpen();
    this->moveStackPointerTo(x);
    this->decrement();
    this->moveStackPointerTo(temp);
    this->increment();
    this->moveStackPointerTo(y);
    this->decrement();
    this->branchClose();

    this->moveStackPointerTo(temp);
    this->branchOpen();
    this->moveStackPointerTo(y);
    this->increment();
    this->moveStackPointerTo(temp);
    this->decrement();
    this->branchClose();

    //Destroy temporary storage and 2nd operand
    this->decrementStackPointerBy(2);

    //Restore the stack pointer
    this->moveStackPointerTo(y);
}

void BrainfuckWriter::subU8()
{
    //Assume stack top contains 2 u8
    size_t x = this->stack_pointer - 2;
    size_t y = this->stack_pointer - 1;
    size_t temp = this->stack_pointer;
    //Create temporary storage
    this->incrementStackPointer();

    //Sub routine
    this->moveStackPointerTo(temp);
    this->clearByte();

    this->moveStackPointerTo(y);
    this->branchOpen();
    this->moveStackPointerTo(x);
    this->increment();
    this->moveStackPointerTo(temp);
    this->increment();
    this->moveStackPointerTo(y);
    this->decrement();
    this->branchClose();

    this->moveStackPointerTo(temp);
    this->branchOpen();
    this->moveStackPointerTo(y);
    this->increment();
    this->moveStackPointerTo(temp);
    this->decrement();
    this->branchClose();

    //Destroy temporary storage and 2nd operator
    this->decrementStackPointerBy(2);

    //Restore the stack pointer
    this->moveStackPointerTo(y);
}

void BrainfuckWriter::mulU8()
{
    //Assume the stack top contains 2 u8
    size_t x = this->stack_pointer - 2;
    size_t y = this->stack_pointer - 1;
    size_t temp = this->stack_pointer;
    size_t temp2 = this->stack_pointer + 1;
    size_t temp3 = this->stack_pointer + 2;
    size_t temp4 = this->stack_pointer + 3;

    //Multiply(x, y) {
    //    temp = y
    //    temp2 = x
    //    x = 0
    //    while(temp) {
    //        x += temp2
    //        --temp;
    //    }
    //}

    //temp = y
    this->loadValue(y, 1);
    //temp2 = x
    this->loadValue(x, 1);
    //x = 0
    this->moveStackPointerTo(x);
    this->clearByte();
    //while(temp) {
    this->moveStackPointerTo(temp);
    this->branchOpen();
    //x += temp2
    this->moveStackPointerTo(temp3);
    this->loadValue(x, 1);
    this->loadValue(temp2, 1);
    this->addU8();
    this->copyByte(temp3, x, temp4);
    //--temp
    this->moveStackPointerTo(temp);
    this->decrement();
    //}
    this->branchClose();

    //Destroy the temporaries + 2nd operand
    this->moveStackPointerTo(y);
}

void BrainfuckWriter::unimplemented()
{
    std::ostream& out = this->getOutput();
    out << "u";
}
