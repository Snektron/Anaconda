#ifndef SRC_EXCEPT_EXCEPTIONS_H_
#define SRC_EXCEPT_EXCEPTIONS_H_

#include <stdexcept>
#include <string>

class AlipheeseException : public std::runtime_error
{
    public:
        AlipheeseException(const std::string& msg);
        AlipheeseException(const char* msg);
        virtual ~AlipheeseException() = default;
};

class ASTException : public AlipheeseException
{
    public:
        ASTException(const std::string& msg);
        ASTException(const char* msg);
        virtual ~ASTException() = default;
};

class TypeCheckException : public ASTException
{
    public:
        TypeCheckException(const std::string& msg);
        TypeCheckException(const char* msg);
        virtual ~TypeCheckException() = default;
};

class RedefinitionException : public ASTException
{
    public:
        RedefinitionException(const std::string& msg);
        RedefinitionException(const char* msg);
        virtual ~RedefinitionException() = default;
};

class TypeMismatchException : public TypeCheckException
{
    public:
        TypeMismatchException(const std::string& msg);
        TypeMismatchException(const char* msg);
        virtual ~TypeMismatchException() = default;
};

class RecursiveTypeException : public TypeCheckException
{
    public:
        RecursiveTypeException(const std::string& msg);
        RecursiveTypeException(const char* msg);
        virtual ~RecursiveTypeException() = default;
};

class SyntaxException : public AlipheeseException
{
    public:
        SyntaxException(const std::string& msg);
        SyntaxException(const char* msg);
        virtual ~SyntaxException() = default;
};

class VariantException : public AlipheeseException
{
    public:
        VariantException(const std::string& msg);
        VariantException(const char* msg);
        virtual ~VariantException() = default;
};

#endif
