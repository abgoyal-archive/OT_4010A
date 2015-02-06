
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L _ T R A C E

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_TRACE_H_INCLUDED
#define OSCL_TRACE_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

class Oscl_Trace
{
public:
    static void begin();
    static void end();
};

#endif // OSCL_TRACE_H_INCLUDED

/*! @} */
