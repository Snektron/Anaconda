#include "datatype.h"

#include <iostream>

const char* DATATYPE_NAMES[] = {
    "void",
    "u8",
    "struct"
};

DataTypeBase::DataTypeBase(DataTypeClass type):
	type(type) {}

DataType<DataTypeClass::STRUCT_FORWARD>::DataType(const std::string& name):
	DataTypeBase(DataTypeClass::STRUCT_FORWARD), name(name) {}

template <>
bool DataType<DataTypeClass::VOID>::isBoolean() const
{
    return false;
}

DataType<DataTypeClass::STRUCT_FORWARD>* DataType<DataTypeClass::STRUCT_FORWARD>::copy() const
{
    return new DataType<DataTypeClass::STRUCT_FORWARD>(this->name);
}

void DataType<DataTypeClass::STRUCT_FORWARD>::print(std::ostream& os) const
{
    os << "struct " << this->name;
}

bool DataType<DataTypeClass::STRUCT_FORWARD>::equals(const DataTypeBase& other) const
{
    if(other.type == DataTypeClass::STRUCT_FORWARD)
        return this->name == ((const DataType<DataTypeClass::STRUCT_FORWARD>&)other).name;
    return false;
}

bool DataType<DataTypeClass::STRUCT_FORWARD>::isBoolean() const
{
    return false;
}

std::ostream& operator<<(std::ostream& os, const DataTypeBase& datatype)
{
    datatype.print(os);
    return os;
}
