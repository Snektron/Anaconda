#include "common/field.h"

Field::Field(DataTypeBase* type, const std::string& name):
	type_(type), name_(name) {}

Field::~Field()
{
	delete type_;
}

const DataTypeBase* Field::type() const
{
	return type_;
}

const std::string& Field::name() const
{
	return name_;
}

Field* Field::copy() const {
	return new Field(type_->copy(), name_);
}
