
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L _ S T R I N G _ U R I

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_STRING_URI_H
#define OSCL_STRING_URI_H

// - - Inclusion - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef OSCL_STRING_H_INCLUDED
#include "oscl_string.h"
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Function prototypes
OSCL_IMPORT_REF bool  oscl_str_unescape_uri(const char *str_buf_in, char *str_buf_out, uint32 max_out_buf_bytes, uint32 max_bytes, uint32& out_buf_len);
//Old Definition
//bool  oscl_str_unescape_uri(const OSCL_String<char>& oscl_str_in, OSCL_String<char>& oscl_str_out, uint32& out_buf_len);
//New definition
OSCL_IMPORT_REF bool  oscl_str_unescape_uri(const OSCL_String& oscl_str_in, OSCL_String& oscl_str_out, uint32& out_buf_len);
#endif

/*! @} */
