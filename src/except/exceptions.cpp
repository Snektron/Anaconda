#include "except/exceptions.h"

AnacondaException::AnacondaException(const std::string& msg):
	std::runtime_error(msg) {}

AnacondaException::AnacondaException(const char* msg):
	std::runtime_error(msg) {}

ASTException::ASTException(const std::string& msg):
	AnacondaException(msg) {}

ASTException::ASTException(const char* msg):
	AnacondaException(msg) {}

TypeCheckException::TypeCheckException(const std::string& msg):
	ASTException(msg) {}

TypeCheckException::TypeCheckException(const char* msg):
	ASTException(msg) {}

RedefinitionException::RedefinitionException(const std::string& msg):
	ASTException(msg) {}

RedefinitionException::RedefinitionException(const char* msg):
	ASTException(msg) {}

RecursiveTypeException::RecursiveTypeException(const std::string& msg):
	TypeCheckException(msg) {}

RecursiveTypeException::RecursiveTypeException(const char* msg):
	TypeCheckException(msg) {}

SyntaxException::SyntaxException(const std::string& msg):
	TypeCheckException(msg) {}

SyntaxException::SyntaxException(const char* msg):
	TypeCheckException(msg) {}
