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

DataType<DataTypeClass::STRUCT>* DataType<DataTypeClass::STRUCT>::copy() const
{
    std::map<std::string, DataTypeBase*> new_map;
    for(auto& it : this->members)
    {
        new_map[it.first] = it.second->copy();
    }
    return new DataType<DataTypeClass::STRUCT>(this->name, new_map);
}

void DataType<DataTypeClass::STRUCT>::print(std::ostream& os) const
{
    os << "struct " << this->name;
}

DataType<DataTypeClass::STRUCT_FORWARD>::DataType(const std::string& name) : DataTypeBase(DataTypeClass::STRUCT_FORWARD), name(name) {}

DataType<DataTypeClass::STRUCT_FORWARD>* DataType<DataTypeClass::STRUCT_FORWARD>::copy() const
{
    return new DataType<DataTypeClass::STRUCT_FORWARD>(this->name);
}

void DataType<DataTypeClass::STRUCT_FORWARD>::print(std::ostream& os) const
{
    os << "struct " << this->name;
}

std::ostream& operator<<(std::ostream& os, const DataTypeBase& datatype)
{
    datatype.print(os);
    return os;
}
