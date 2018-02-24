#ifndef SRC_TYPES_DATATYPE_H_
#define SRC_TYPES_DATATYPE_H_

#include <string>
#include <map>
#include <iosfwd>

class BrainfuckWriter;

enum class DataTypeClass
{
    VOID,
    U8,
    STRUCT_FORWARD
};

extern const char* DATATYPE_NAMES[];
extern const size_t DATATYPE_SIZES[];

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
        virtual bool supportsArithmetic() const = 0;
        virtual bool canCastFrom(const DataTypeBase&) const = 0;
        virtual size_t size(BrainfuckWriter&) const = 0;
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
        virtual bool supportsArithmetic() const;
        virtual bool canCastFrom(const DataTypeBase&) const;
        virtual size_t size(BrainfuckWriter&) const;
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
        virtual bool supportsArithmetic() const;
        virtual bool canCastFrom(const DataTypeBase&) const;
        virtual size_t size(BrainfuckWriter&) const;
};

template<>
bool DataType<DataTypeClass::VOID>::isBoolean() const;
template<>
bool DataType<DataTypeClass::VOID>::supportsArithmetic() const;
template<>
bool DataType<DataTypeClass::VOID>::canCastFrom(const DataTypeBase&) const;

std::ostream& operator<<(std::ostream&, const DataTypeBase&);

#include "datatype.inl"

#endif
