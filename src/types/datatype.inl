#include <iostream>
#include "common/util.h"

template <DataTypeClass dtype>
DataType<dtype>::DataType() : DataTypeBase(dtype) {}

template <DataTypeClass dtype>
void DataType<dtype>::print(std::ostream& os) const
{
    os << DATATYPE_NAMES[(size_t)dtype];
}

template <DataTypeClass dtype>
DataType<dtype>* DataType<dtype>::copy() const
{
    return new DataType<dtype>();
}

template <DataTypeClass dtype>
bool DataType<dtype>::equals(const DataTypeBase& other) const
{
    return other.type == dtype;
}

template <DataTypeClass dtype>
bool DataType<dtype>::isBoolean() const
{
    return true;
}

template <DataTypeClass dtype>
bool DataType<dtype>::supportsArithmetic() const
{
    return true;
}

template <DataTypeClass dtype>
bool DataType<dtype>::canCastFrom(const DataTypeBase& type) const
{
    return type.supportsArithmetic();
}

template <DataTypeClass dtype>
size_t DataType<dtype>::size(BrainfuckWriter& writer) const
{
    UNUSED(writer);
    return DATATYPE_SIZES[(size_t)dtype];
}
