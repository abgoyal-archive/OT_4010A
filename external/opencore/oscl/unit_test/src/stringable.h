
#ifndef STRINGABLE_H
#define STRINGABLE_H

#include "unit_test_common.h"

class stringable
{
    public:
        virtual _STRING to_string(void) const = 0;
        virtual ~stringable() {}
};

template< class TT >
inline
_STRING valueToString(const TT &)
{
    return "<value not stringable>";
}

inline
_STRING valueToString(const bool & value)
{
    return (true == value) ? "true" : "false";
}

inline
_STRING valueToString(const int & value)
{
    char buffer[32];
    sprintf(buffer, "%d", value);
    return buffer;
}

#endif
