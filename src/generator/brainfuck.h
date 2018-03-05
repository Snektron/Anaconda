#ifndef SRC_GENERATOR_BRAINFUCK_H_
#define SRC_GENERATOR_BRAINFUCK_H_

#include <iosfwd>
#include <map>
#include <vector>

#include "types/datatype.h"
#include "ast/node.h"
#include "common/field.h"
#include "ast/stat/blocknode.h"

const size_t GLOBAL_SCOPE = 0;

class Scope
{
    private:
        std::vector<std::map<std::string, const DataTypeBase*>> declarations;
        std::vector<std::map<std::string, size_t>> stack_locations;
    public:
        Scope() = default;
        Scope(const Scope&) = delete;
        Scope(Scope&&);
        ~Scope() = default;

        Scope& operator=(const Scope&) = delete;

        //Declares a variable in the currently active frame
        void declareVariable(const std::string&, const DataTypeBase* datatype);
        void setVariableLocation(const std::string&, size_t);

        //Variable search
        const DataTypeBase* findVariable(const std::string&);
        size_t findVariableLocation(const std::string&);

        //Checks
        bool hasVariable(const std::string&);
        bool hasFrameVariable(const std::string&);

        //Frame control
        void enterFrame();
        void exitFrame();

        //Scope fetch
        std::map<std::string, const DataTypeBase*>& getFrameDeclarations();
};

class FunctionDefinition
{
    private:
        std::vector<Field> arguments;
        const DataTypeBase* return_type;
        BlockNode* code;
    public:
        FunctionDefinition(const std::vector<Field>&, const DataTypeBase*, BlockNode*);
        FunctionDefinition(FunctionDefinition&&);
        FunctionDefinition(const FunctionDefinition&) = delete;
        ~FunctionDefinition() = default;

        FunctionDefinition& operator=(const FunctionDefinition&) = delete;

        bool parametersEqual(const std::vector<DataTypeBase*>& arguments);
        DataTypeBase* getReturnType() const;
};

class StructureDefinition
{
    public:
        std::vector<Field> fields;
    public:
        StructureDefinition(const std::vector<Field>& fields);
        StructureDefinition(StructureDefinition&&);
        StructureDefinition(const StructureDefinition&) = delete;
        ~StructureDefinition() = default;

        StructureDefinition& operator=(const StructureDefinition&) = delete;

        size_t size(BrainfuckWriter&) const;
};

class VariableDefinition
{
    public:
        const DataTypeBase* datatype;
        size_t stack_location;
    public:
        VariableDefinition(const DataTypeBase*, size_t);
        ~VariableDefinition() = default;

        size_t location() const;
        DataTypeBase* dataType() const;
};

class BrainfuckWriter
{
    private:
        std::ostream* output;

        std::vector<Scope> scopes;
        std::multimap<std::string, FunctionDefinition> functions;
        std::map<std::string, StructureDefinition> structures;
        std::vector<FunctionDefinition*> scope_func_lookup;
        size_t current_scope;

        size_t stack_pointer;
    public:
        BrainfuckWriter(std::ostream&);
        ~BrainfuckWriter() = default;

        //Declarations
        size_t declareFunction(const std::string&, const std::vector<Field>&, const DataTypeBase*, BlockNode*);
        void declareStructure(const std::string&, const std::vector<Field>&);
        void declareVariable(const std::string&, const DataTypeBase*);

        //Checks
        bool isFunctionDeclared(const std::string&, const std::vector<Field>&);
        bool isFunctionDeclared(const std::string&, const std::vector<DataTypeBase*>&);
        bool isStructureDeclared(const std::string&);

        //Lookup operations
        FunctionDefinition* getDeclaredFunction(const std::string&, const std::vector<DataTypeBase*>&);
        StructureDefinition* getDeclaredStructure(const std::string&);
        VariableDefinition* getDeclaredVariable(const std::string&);

        //Controlling function-level scope
        void switchScope(size_t);
        size_t getScope();
        FunctionDefinition* lookupScope(size_t);

        //Controlling statement-level frames
        void enterFrame();
        void exitFrame();

        //Output control
        std::ostream& getOutput();
        std::ostream& setOutput(std::ostream&);

        //Stack location
        size_t getStackLocation();

        //Code generation functions
        //Raw assembly copy
        void copyAssembly(const std::string&);
        //Basic variable arithmetic
        void increment();
        void decrement();
        void incrementBy(size_t);
        void decrementBy(size_t);
        //Basic stack manipulation
        void incrementStackPointer();
        void decrementStackPointer();
        void incrementStackPointerBy(size_t);
        void decrementStackPointerBy(size_t);
        void moveStackPointerTo(size_t);
        //Basic control flow structures
        void branchOpen();
        void branchClose();
        //Scope manipulation
        void makeStackFrame();
        void destroyStackFrame();
        //Advanced stack manipulation
        void push(const DataTypeBase*);
        void pop(const DataTypeBase*);
        //Constants
        void pushByte(uint8_t);
        //Advanced value manipulation
        void clearByte();
        void copyByte(size_t, size_t, size_t);
        void copyValue(size_t, size_t, size_t, size_t);
        void loadValue(size_t, size_t);
        //8-bit unsigned arithmetic
        void addU8();
        void subU8();
        void mulU8();

        void unimplemented();
};

#endif
