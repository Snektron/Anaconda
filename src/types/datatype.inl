#include <iostream>

template <DataTypeClass dtype>
DataType<dtype>::DataType() : DataTypeBase(type) {}

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
