
#ifndef PVRTSP_CLIENT_ENGINE_UTILS_H
#define PVRTSP_CLIENT_ENGINE_UTILS_H
//#include "pvrtsp_client_engine_utils.h"

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_STDSTRING_H_INCLUDED
#include "oscl_stdstring.h"
#endif

#ifndef OSCL_UTF8CONV_H
#include "oscl_utf8conv.h"
#endif

#ifndef OSCL_ERROR_CODES_H_INCLUDED
#include "oscl_error_codes.h"
#endif

#ifndef OSCL_STRING_H_INCLUDED
#include "oscl_string.h"
#endif

#define MAX_LONG_TEXT_LEN 1024

typedef enum
{
    CONCATENATE,
    REPLACE_PATH,
    REPLACE_HOST,
    UNKNOWN
} URLType;

const char FWD_SLASH = '/';
const char COLON = ':';
const char DOT = '.';


bool composeURL(const char *baseURL, const char *relativeURL, \
                char *completeURL, unsigned int &completeURLLen);

const char* findRelativeURL(const char *aURL);

URLType findRelativeURLType(const char *relativeURL);

void dropTextAfterLastSlash(char *copyOfBaseURL);

void dropTextAfterFirstSlash(char *copyOfBaseURL);

bool generatePseudoUUID(OSCL_String& aUUID);

#endif  //PVRTSP_CLIENT_ENGINE_UTILS_H
