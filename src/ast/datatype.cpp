#include "datatype.h"

DataTypeBase::DataTypeBase(DataTypeClass type) : type(type) {}

DataType<DATATYPE_STRUCT>::DataType(const std::string& name, const std::map<std::string, DataTypeBase*>& members) : name(name), members(members) {}

DataType<DATATYPE_STRUCT>::~DataType()
{
    for(auto& it : this->members)
        delete this->members.second;
}
