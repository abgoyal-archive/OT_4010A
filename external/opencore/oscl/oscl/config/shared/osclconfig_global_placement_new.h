
#ifndef OSCLCONFIG_GLOBAL_PLACEMENT_NEW_H_INCLUDED
#define OSCLCONFIG_GLOBAL_PLACEMENT_NEW_H_INCLUDED

//This file contains a definition of placement new operator
//for use in configurations without a native placement new
//operator definition.

inline void* operator new(size_t, void* ptr)
{
    return ptr;
}


#endif
