
// -*- c++ -*-

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
//                     PV_MIME_STRING_UTILS
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =


#ifndef OSCL_MIME_STRING_UTILS_H
#define OSCL_MIME_STRING_UTILS_H

// - - Inclusion - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_STDSTRING_H_INCLUDED
#include "oscl_stdstring.h"
#endif

#ifndef OSCL_BASE_MACROS_H_INCLUDED
#include "oscl_base_macros.h"
#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char * pv_mime_string_append_type(char *mimestring, char *string, char* newstring);


char * pv_mime_string_append_param(char *mimestring, char *string, char* newstring);


int pv_mime_string_parse_type(char *mimestring, char *string, char * &stringaddr);

OSCL_IMPORT_REF int pv_mime_string_parse_param(char *mimestring, char *string, char * &stringaddr);

OSCL_IMPORT_REF int pv_mime_strcmp(const char *mimestring1, const char *mimestring2);

OSCL_IMPORT_REF int pv_mime_strstr(char *mimestring1, char *mimestring2);

OSCL_IMPORT_REF int pv_mime_string_extract_type(int compnumber, char *mimestring, char * &string);

OSCL_IMPORT_REF int pv_mime_string_extract_param(int compnumber, char *mimestring, char * &string);


OSCL_IMPORT_REF int pv_mime_string_compcnt(char* mimestring);


OSCL_IMPORT_REF int pv_mime_string_parcnt(char* mimestring);


bool pv_mime_string_is_relative(char* mimestring);


char * pv_mime_string_strappend(char *mimestring, char *rel_mimestring, char* newstring);

#endif
