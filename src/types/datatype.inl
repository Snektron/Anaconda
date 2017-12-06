#include <iostream>

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
