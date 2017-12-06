#ifndef DATATYPE_H_INCLUDED
#define DATATYPE_H_INCLUDED

#include <string>
#include <map>
#include <iosfwd>

enum class DataTypeClass
{
    VOID,
    U8,
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
        virtual bool equals(const DataTypeBase&) const = 0;
        virtual bool isBoolean() const = 0;
};

template <DataTypeClass dtype>
class DataType : public DataTypeBase
{
    public:
        DataType();
        virtual ~DataType() = default;

        virtual DataType<dtype>* copy() const;
        virtual void print(std::ostream&) const;
        virtual bool equals(const DataTypeBase&) const;
        virtual bool isBoolean() const;
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
        virtual bool equals(const DataTypeBase&) const;
        virtual bool isBoolean() const;
};

template<>
bool DataType<DataTypeClass::VOID>::isBoolean() const;

std::ostream& operator<<(std::ostream&, const DataTypeBase&);

#include "datatype.inl"

#endif
