
#ifndef OSCLCONFIG_GLOBAL_NEW_DELETE_H_INCLUDED
#define OSCLCONFIG_GLOBAL_NEW_DELETE_H_INCLUDED

//This file contains overloads for the global new/delete operators
//for use in configurations without a native new/delete operator,
//or where it is desirable to overload the existing global new/delete
//operators.  The implementation of the operators is in oscl_mem.cpp.

void* operator new(size_t);
void operator delete(void*);


#endif
