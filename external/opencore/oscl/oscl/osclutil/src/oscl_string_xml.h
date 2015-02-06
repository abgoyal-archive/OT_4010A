
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L _ S T R I N G _ X M L

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =





#ifndef OSCL_STRING_XML_H
#define OSCL_STRING_XML_H

// - - Inclusion - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Function prototypes
OSCL_IMPORT_REF bool  oscl_str_need_escape_xml(const char *str_buf, uint32& num_escape_bytes, uint32 max_bytes = 0);
OSCL_IMPORT_REF int32  oscl_str_escape_xml(const char *str_buf_in, char *str_buf_out, uint32 max_out_buf_bytes, uint32 max_bytes = 0,
        uint32 * num_bytes_written = NULL);
#endif


/*! @} */
