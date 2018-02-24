#ifndef SRC_EXCEPT_EXCEPTIONS_H_
#define SRC_EXCEPT_EXCEPTIONS_H_

#include <stdexcept>
#include <string>

class AnacondaException : public std::runtime_error
{
    public:
        AnacondaException(const std::string& msg);
        AnacondaException(const char* msg);
        virtual ~AnacondaException() = default;
};

class ASTException : public AnacondaException
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

class SyntaxException : public AnacondaException
{
    public:
        SyntaxException(const std::string& msg);
        SyntaxException(const char* msg);
        virtual ~SyntaxException() = default;
};

class VariantException : public AnacondaException
{
    public:
        VariantException(const std::string& msg);
        VariantException(const char* msg);
        virtual ~VariantException() = default;
};

#endif
