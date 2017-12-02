#ifndef DATATYPE_H_INCLUDED
#define DATATYPE_H_INCLUDED

#include <string>
#include <map>
#include <iosfwd>

enum class DataTypeClass
{
    VOID,
    U8,
    STRUCT,
    STRUCT_FORWARD
};

extern const char* DATATYPE_NAMES[];

class DataTypeBase
{
    protected:
        DataTypeBase(DataTypeClass);
    public:
        virtual ~DataTypeBase() = default;
        DataTypeClass type;

        virtual DataTypeBase* copy() const = 0;
        virtual void print(std::ostream&) const = 0;
};

template <DataTypeClass dtype>
class DataType : public DataTypeBase
{
    public:
        DataType();
        virtual ~DataType() = default;

        virtual DataType<dtype>* copy() const;
        virtual void print(std::ostream&) const;
};

template <>
class DataType<DataTypeClass::STRUCT> : public DataTypeBase
{
    public:
        std::string name;
        std::map<std::string, DataTypeBase*> members;

        DataType(const std::string&, const std::map<std::string, DataTypeBase*>&);
        virtual ~DataType();

        virtual DataType<DataTypeClass::STRUCT>* copy() const;
        virtual void print(std::ostream&) const;
};

template<>
class DataType<DataTypeClass::STRUCT_FORWARD> : public DataTypeBase
{
    public:
        std::string name;
    
        DataType(const std::string&);
        virtual ~DataType() = default;

        virtual DataType<DataTypeClass::STRUCT_FORWARD>* copy() const;
        virtual void print(std::ostream&) const;
};

std::ostream& operator<<(std::ostream&, const DataTypeBase&);

#include "datatype.inl"

#endif
