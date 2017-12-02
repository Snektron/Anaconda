#include "datatype.h"

#include <iostream>

const char* DATATYPE_NAMES[] = {
    "void",
    "u8",
    "struct",
    "struct"
};

DataTypeBase::DataTypeBase(DataTypeClass type) : type(type) {}

DataType<DataTypeClass::STRUCT>::DataType(const std::string& name, const std::map<std::string, DataTypeBase*>& members) : DataTypeBase(DataTypeClass::STRUCT), name(name), members(members) {}

DataType<DataTypeClass::STRUCT>::~DataType()
{
    for(auto& it : this->members)
        delete it.second;
}

void DataType<DataTypeClass::STRUCT>::print(std::ostream& os) const
{
    os << "struct " << this->name;
}

DataType<DataTypeClass::STRUCT_FORWARD>::DataType(const std::string& name) : DataTypeBase(DataTypeClass::STRUCT_FORWARD), name(name) {}

void DataType<DataTypeClass::STRUCT_FORWARD>::print(std::ostream& os) const
{
    os << "struct " << this->name;
}

std::ostream& operator<<(std::ostream& os, const DataTypeBase& datatype)
{
    datatype.print(os);
    return os;
}
