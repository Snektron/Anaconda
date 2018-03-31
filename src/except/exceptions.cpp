#include "except/exceptions.h"

AlipheeseException::AlipheeseException(const std::string& msg):
    std::runtime_error(msg) {}

AlipheeseException::AlipheeseException(const char* msg):
    std::runtime_error(msg) {}

ASTException::ASTException(const std::string& msg):
    AlipheeseException(msg) {}

ASTException::ASTException(const char* msg):
    AlipheeseException(msg) {}

TypeCheckException::TypeCheckException(const std::string& msg):
    ASTException(msg) {}

TypeCheckException::TypeCheckException(const char* msg):
    ASTException(msg) {}

RedefinitionException::RedefinitionException(const std::string& msg):
    ASTException(msg) {}

RedefinitionException::RedefinitionException(const char* msg):
    ASTException(msg) {}

TypeMismatchException::TypeMismatchException(const std::string& msg):
    TypeCheckException(msg) {}

TypeMismatchException::TypeMismatchException(const char* msg):
    TypeCheckException(msg) {}

RecursiveTypeException::RecursiveTypeException(const std::string& msg):
    TypeCheckException(msg) {}

RecursiveTypeException::RecursiveTypeException(const char* msg):
    TypeCheckException(msg) {}

SyntaxException::SyntaxException(const std::string& msg):
    AlipheeseException(msg) {}

SyntaxException::SyntaxException(const char* msg):
    AlipheeseException(msg) {}

VariantException::VariantException(const std::string& msg):
    AlipheeseException(msg) {}

VariantException::VariantException(const char* msg):
    AlipheeseException(msg) {}
