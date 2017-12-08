#ifndef SRC_COMMON_FIELD_H_
#define SRC_COMMON_FIELD_H_

#include <ostream>
#include "types/datatype.h"

class Field
{
    private:
        const DataTypeBase* type;
        const std::string name;

        friend std::ostream& operator<<(std::ostream&, const Field&);
    public:
        Field(DataTypeBase* type, const std::string& name);
        ~Field();

        const DataTypeBase* getType() const;
        const std::string& getName() const;

        Field* copy() const;
};

std::ostream& operator<<(std::ostream&, const Field&);

#endif
