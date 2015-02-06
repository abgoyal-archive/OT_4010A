
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L _ S T R I N G _ U T F 8

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_STRING_UTF8_H
#define OSCL_STRING_UTF8_H

// - - Inclusion - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Function prototypes
OSCL_IMPORT_REF  bool  oscl_str_is_valid_utf8(const uint8 *str_buf, uint32& num_valid_characters, uint32 max_bytes = 0,
        uint32 max_char_2_valid = 0, uint32 * num_byte_4_char = NULL);
OSCL_IMPORT_REF int32  oscl_str_truncate_utf8(uint8 *str_buf, uint32 max_char, uint32 max_bytes = 0);

#endif

/*! @} */
