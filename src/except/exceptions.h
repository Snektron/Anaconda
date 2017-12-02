#ifndef EXCEPTIONS_H_INCLUDED
#define EXCEPTIONS_H_INCLUDED

#include <stdexcept>

class ProgramException : public std::runtime_error
{
    public:
        ProgramException(const std::string&);
        ProgramException(const char*);
        virtual ~ProgramException() = default;
};

class ASTException : public ProgramException
{
    public:
        ASTException(const std::string&);
        ASTException(const char*);
        virtual ~ASTException() = default;
};

class TypeCheckException : public ASTException
{
    public:
        TypeCheckException(const std::string&);
        TypeCheckException(const char*);
        virtual ~TypeCheckException() = default;
};

class RedefinitionException : public ASTException
{
    public:
        RedefinitionException(const std::string&);
        RedefinitionException(const char*);
        virtual ~RedefinitionException() = default;
};

class RecursiveTypeException : public TypeCheckException
{
    public:
        RecursiveTypeException(const std::string&);
        RecursiveTypeException(const char*);
        virtual ~RecursiveTypeException() = default;
};

#endif
