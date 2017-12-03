#ifndef SRC_COMMON_FIELD_H_
#define SRC_COMMON_FIELD_H_

#include "types/datatype.h"

class Field
{
private:
	const DataTypeBase *type_;
	const std::string name_;
public:
	Field(DataTypeBase* type, const std::string& name);
	~Field();

	const DataTypeBase* type() const;
	const std::string& name() const;

	Field* copy() const;
};

#endif
