#include "common/field.h"

Field::Field(DataTypeBase* type, const std::string& name):
	type(type), name(name) {}

Field::~Field()
{
	delete this->type;
}

const DataTypeBase* Field::getType() const
{
	return this->type;
}

const std::string& Field::getName() const
{
	return this->name;
}

Field* Field::copy() const {
	return new Field(this->type->copy(), this->name);
}
