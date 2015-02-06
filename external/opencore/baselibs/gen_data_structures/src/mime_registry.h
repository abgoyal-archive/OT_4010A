
#ifndef SDP_REGISTRAR_H
#define SDP_REGISTRAR_H
//#include "osclconfig_limits_typedefs.h"
#include "sdp_error.h"
#include "oscl_str_ptr_len.h"
#include "oscl_string.h"


class SDPInfo;
class mediaInfo;

class registrar
{
    private:
        int parserCount;
        int composerCount;
        char *parserMIME[MAX_CODEC_MODE_LIST];
        char *composerMIME[MAX_CODEC_MODE_LIST];
        SDP_ERROR_CODE(*parserPtr[MAX_CODEC_MODE_LIST])(const char*, const int, SDPInfo *);
        SDP_ERROR_CODE(*composerPtr[MAX_CODEC_MODE_LIST])(char *, mediaInfo *, int32 &, bool);
    public:
        registrar()
        {
            parserCount = 0;
            composerCount = 0;
        }

        ~registrar()
        {
            int ii;
            for (ii = 0; ii < parserCount; ii++)
            {
                OSCL_ARRAY_DELETE(parserMIME[ii]);
                parserMIME[ii] = 0;
            }
            parserCount = 0;
            for (ii = 0; ii < composerCount; ii++)
            {
                OSCL_ARRAY_DELETE(composerMIME[ii]);
                composerMIME[ii] = 0;
            }
            composerCount = 0;
        }

        bool
        registerSDPParser(const char *n, const int len, SDP_ERROR_CODE(*p)(const char*, const int, SDPInfo *))
        {
            if (parserCount < MAX_CODEC_MODE_LIST)
            {
                if ((parserMIME[parserCount] = OSCL_ARRAY_NEW(char, (len + 1))) != NULL)
                {
                    oscl_strncpy(parserMIME[parserCount], n, len);
                    parserMIME[parserCount][len] = '\0';
                    parserPtr[parserCount] = p;
                    parserCount++;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        bool
        registerSDPComposer(const char *n, const int len, SDP_ERROR_CODE(*p)(char*, mediaInfo *, int32 &, bool))
        {
            if (composerCount < MAX_CODEC_MODE_LIST)
            {
                if ((composerMIME[composerCount] = OSCL_ARRAY_NEW(char, len + 1)) != NULL)
                {
                    oscl_strncpy(composerMIME[composerCount], n, len);
                    composerMIME[composerCount][len] = '\0';
                    composerPtr[composerCount] = p;
                    composerCount++;
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        SDP_ERROR_CODE
        (*lookupSDPParser(char * n, int len))(const char*, const int, SDPInfo *)
        {
            int ii;
            for (ii = 0; ii < parserCount; ii++)
            {
                if (!oscl_strncmp(parserMIME[ii], n, len))
                {
                    return parserPtr[ii];
                }
            }
            return NULL;
        }

        SDP_ERROR_CODE
        (*lookupSDPComposer(char * n, int len))(char*, mediaInfo *, int32 &, bool)
        {
            int ii;
            for (ii = 0; ii < composerCount; ii++)
            {
                if (!oscl_strncmp(composerMIME[ii], n, len))
                {
                    return composerPtr[ii];
                }
            }
            return NULL;
        }
};

#endif //SDP_REGISTRAR_H
