#include "datatype.h"

DataTypeBase::DataTypeBase(DataTypeClass type) : type(type) {}

DataType<DATATYPE_STRUCT>::DataType(const std::string& name, const std::map<std::string, DataTypeBase*>& members) : DataTypeBase(DATATYPE_STRUCT), name(name), members(members) {}

DataType<DATATYPE_STRUCT>::~DataType()
{
    for(auto& it : this->members)
        delete it.second;
}

DataType<DATATYPE_STRUCT_FORWARD>::DataType(const std::string& name) : DataTypeBase(DATATYPE_STRUCT_FORWARD), name(name) {}
