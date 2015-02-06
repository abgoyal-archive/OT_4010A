
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L   B Y T E O R D E R   U T I L S

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =





#ifndef OSCL_BYTE_ORDER_H_INCLUDED
#define OSCL_BYTE_ORDER_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif


//! Convert little endian to host format.
void little_endian_to_host(char *data, uint32 size);

//! Convert host to little endian format.
void host_to_little_endian(char *data, unsigned int size);

//! Convert big endian to host format.
void big_endian_to_host(char *data, unsigned int size);

//! Convert host to big endian format.
void host_to_big_endian(char *data, unsigned int size);

#if (!OSCL_DISABLE_INLINES)
#include "oscl_byte_order.inl"
#endif

/*! @} */

#endif
