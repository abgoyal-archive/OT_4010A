
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L _ U T F 8 C O N V

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =







#ifndef OSCL_UTF8CONV_H
#define OSCL_UTF8CONV_H

#ifndef OSCL_BASE_INCLUDED_H
#include "oscl_base.h"
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Function prototypes

OSCL_IMPORT_REF int32 oscl_UTF8ToUnicode(const char *input, int32 inLength, oscl_wchar *output, int32 outLength);


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Function prototypes

OSCL_IMPORT_REF int32 oscl_UnicodeToUTF8(const oscl_wchar *input, int32 inLength, char *output, int32 outLength);

#endif /* OSCL_UTF8CONV_H */

/*! @} */
