#ifndef DATATYPE_H_INCLUDED
#define DATATYPE_H_INCLUDED

#include <string>
#include <map>

enum DataTypeClass
{
    DATATYPE_VOID,
    DATATYPE_U8,
    DATATYPE_STRUCT
};

class DataTypeBase
{
    protected:
        DataTypeBase(DataTypeClass);
        virtual ~DataTypeBase();
    public:
        DataTypeClass type;
};

template <DataTypeClass type>
class DataType : public DataTypeBase
{
    public:
        DataType();
        virtual ~DataType();
};

template <>
class DataType<DATATYPE_STRUCT> : public DataTypeBase
{
    public:
        std::map<std::string, DataType*> members;

        DataType(const std::map<std::string, DataType*>&);
        virtual ~DataType();
};

#endif
