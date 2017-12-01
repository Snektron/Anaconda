#ifndef DATATYPE_H_INCLUDED
#define DATATYPE_H_INCLUDED

#include <string>
#include <map>

enum DataTypeClass
{
    DATATYPE_VOID,
    DATATYPE_U8,
    DATATYPE_STRUCT,
    DATATYPE_STRUCT_FORWARD
};

class DataTypeBase
{
    protected:
        DataTypeBase(DataTypeClass);
    public:
        virtual ~DataTypeBase() = default;
        DataTypeClass type;
};

template <DataTypeClass type>
class DataType : public DataTypeBase
{
    public:
        DataType();
        virtual ~DataType() = default;
};

template <>
class DataType<DATATYPE_STRUCT> : public DataTypeBase
{
    public:
        std::string name;
        std::map<std::string, DataTypeBase*> members;

        DataType(const std::string&, const std::map<std::string, DataTypeBase*>&);
        virtual ~DataType();
};

template<>
class DataType<DATATYPE_STRUCT_FORWARD> : public DataTypeBase
{
    public:
        std::string name;
    
        DataType(const std::string&);
        virtual ~DataType() = default;
};

#include "datatype.inl"

#endif
