
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                     O S C L _ S T D S T R I N G

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =





#ifndef OSCL_STDSTRING_H_INCLUDED
#define OSCL_STDSTRING_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif


OSCL_IMPORT_REF uint32 oscl_strlen(const char* str);

OSCL_IMPORT_REF uint32 oscl_strlen(const oscl_wchar* str);

OSCL_IMPORT_REF char* oscl_strncpy(char* dest, const char* src, uint32 count);

OSCL_IMPORT_REF oscl_wchar* oscl_strncpy(oscl_wchar* dest, const oscl_wchar* src, uint32 count);

OSCL_IMPORT_REF int32 oscl_strcmp(const char* str1, const char* str2);

OSCL_IMPORT_REF int32 oscl_strcmp(const oscl_wchar* str1, const oscl_wchar* str2);


OSCL_IMPORT_REF int32 oscl_strncmp(const char* str1, const char* str2, uint32 count);

OSCL_IMPORT_REF int32 oscl_strncmp(const oscl_wchar* str1, const oscl_wchar* str2, uint32 count);

OSCL_IMPORT_REF char* oscl_strncat(char* dest, const char* src, uint32 count);

OSCL_IMPORT_REF oscl_wchar* oscl_strncat(oscl_wchar* dest, const oscl_wchar* src, uint32 count);

OSCL_IMPORT_REF const char* oscl_strchr(const char *str, int32 c);
OSCL_IMPORT_REF char* oscl_strchr(char *str, int32 c);

OSCL_IMPORT_REF const oscl_wchar* oscl_strchr(const oscl_wchar *str, int32 c);
OSCL_IMPORT_REF oscl_wchar* oscl_strchr(oscl_wchar *str, int32 c);

OSCL_IMPORT_REF const char* oscl_strrchr(const char *str, int32 c);
OSCL_IMPORT_REF char* oscl_strrchr(char *str, int32 c);
OSCL_IMPORT_REF const oscl_wchar* oscl_strrchr(const oscl_wchar *str, int32 c);
OSCL_IMPORT_REF oscl_wchar* oscl_strrchr(oscl_wchar *str, int32 c);

OSCL_IMPORT_REF char* oscl_strset(char* dest, char val, uint32 count);

OSCL_IMPORT_REF oscl_wchar* oscl_strset(oscl_wchar* dest, oscl_wchar val, uint32 count);


OSCL_IMPORT_REF int32 oscl_CIstrcmp(const char *str1, const char *str2);

OSCL_IMPORT_REF int32 oscl_CIstrcmp(const oscl_wchar *str1, const oscl_wchar *str2);

OSCL_IMPORT_REF int32 oscl_CIstrncmp(const char *str1, const char *str2, uint32 count);

OSCL_IMPORT_REF int32 oscl_CIstrncmp(const oscl_wchar *str1, const oscl_wchar *str2, uint32 count);

OSCL_IMPORT_REF char oscl_tolower(const char car);


OSCL_IMPORT_REF oscl_wchar oscl_tolower(const oscl_wchar car);


OSCL_IMPORT_REF bool oscl_isLetter(const char car);


OSCL_IMPORT_REF const char* oscl_strstr(const char* str1, const char* str2);
OSCL_IMPORT_REF char* oscl_strstr(char* str1, const char* str2);


OSCL_IMPORT_REF const oscl_wchar* oscl_strstr(const oscl_wchar* str1, const oscl_wchar* str2);
OSCL_IMPORT_REF oscl_wchar* oscl_strstr(oscl_wchar* str1, const oscl_wchar* str2);


OSCL_IMPORT_REF char* oscl_strcat(char* dest, const char* src);


OSCL_IMPORT_REF oscl_wchar* oscl_strcat(oscl_wchar* dest, const oscl_wchar* src);


/*! @} */


#endif
