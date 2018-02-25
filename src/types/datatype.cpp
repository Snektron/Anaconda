#include "datatype.h"
#include "common/util.h"
#include "generator/brainfuck.h"

#include <iostream>

const char* DATATYPE_NAMES[] = {
    "void",
    "u8",
    "struct"
};

const size_t DATATYPE_SIZES[] = {
    0,
    1,
    0
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

template <>
bool DataType<DataTypeClass::VOID>::supportsArithmetic() const
{
    return false;
}

template<>
bool DataType<DataTypeClass::VOID>::canCastFrom(const DataTypeBase& type) const
{
    UNUSED(type);
    return true;
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

bool DataType<DataTypeClass::STRUCT_FORWARD>::supportsArithmetic() const
{
    return false;
}

bool DataType<DataTypeClass::STRUCT_FORWARD>::canCastFrom(const DataTypeBase& type) const
{
    UNUSED(type);
    return false;
}

size_t DataType<DataTypeClass::STRUCT_FORWARD>::size(BrainfuckWriter& writer) const
{
    return writer.getDeclaredStructure(this->name)->size(writer);
}

std::ostream& operator<<(std::ostream& os, const DataTypeBase& datatype)
{
    datatype.print(os);
    return os;
}
