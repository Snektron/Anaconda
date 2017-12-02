#include <iostream>

template <DataTypeClass type>
DataType<type>::DataType() : DataTypeBase(type) {}

template <DataTypeClass type>
void DataType<type>::print(std::ostream& os) const
{
    os << DATATYPE_NAMES[(size_t)type];
}
