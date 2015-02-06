

#include "oscl_utf8conv.h"

#define BYTE_1_REP          0x80    /* if <, will be represented in 1 byte */
#define BYTE_2_REP          0x800   /* if <, will be represented in 2 bytes */

#define SURROGATE_MIN       0xd800
#define SURROGATE_MAX       0xdfff


#define SIGMASK_2_1                0xc0
#define SIGMASK_3_1                0xe0



/**********************************************************************************/
/*                                                                                */
/* Function:     UnicodeToUTF8                                                    */
/* Description:  Convert Unicode string to UTF8 byte sequence                     */
/*                                                                                */
/* Parameters:   szSrc - Unicode string to be converted                           */
/*                                 nSrcLen - Length of szSrc                      */
/*               strDest - char buffer for UTF8 text                              */
/*               nDestLen - size (in characters) of buffer                        */
/*                                                                                */
/* Returns:      On success, the number of bytes in the destination buffer        */
/*               0 on failure due to insufficient buffer size                     */
/*                                                                                */
/* History:      Created  {DATE]  {BY} {NAME} {PRODUCT REV}                       */
/*               Modified {DATE]  {BY} {NAME} {PRODUCT REV}                       */
/*                                                                                */
/**********************************************************************************/

OSCL_EXPORT_REF int32 oscl_UnicodeToUTF8(const oscl_wchar *szSrc, int32 nSrcLen, char *strDest, int32 nDestLen)
{
    int32 i = 0;
    int32 i_cur_output = 0;
    char ch_tmp_byte;

    if (nDestLen <= 0)
    {
        // We cannot append terminate 0 at this case.
        return 0; /* ERROR_INSUFFICIENT_BUFFER */
    }

    for (i = 0; i < nSrcLen; i++)
    {
        if (BYTE_1_REP > szSrc[i]) /* 1 byte utf8 representation */
        {
            if (i_cur_output + 1 < nDestLen)
            {
                strDest[i_cur_output++] = (char)szSrc[i];
            }
            else
            {
                strDest[i_cur_output] = '\0'; /* Terminate string */
                return 0; /* ERROR_INSUFFICIENT_BUFFER */
            }
        }
        else if (BYTE_2_REP > szSrc[i]) /* 2 byte utf8 representation */
        {
            if (i_cur_output + 2 < nDestLen)
            {
                strDest[i_cur_output++] = (char)(szSrc[i] >> 6 | 0xc0);
                strDest[i_cur_output++] = (char)((szSrc[i] & 0x3f) | 0x80);
            }
            else
            {
                strDest[i_cur_output] = '\0'; /* Terminate string */
                return 0; /* ERROR_INSUFFICIENT_BUFFER */
            }
        }
        else if (SURROGATE_MAX > szSrc[i] && SURROGATE_MIN < szSrc[i])
        {        /* 4 byte surrogate pair representation */
            if (i_cur_output + 4 < nDestLen)
            {
                ch_tmp_byte = (char)(((szSrc[i] & 0x3c0) >> 6) + 1);
                strDest[i_cur_output++] = (char)(ch_tmp_byte >> 2 | 0xf0);
                strDest[i_cur_output++] = (char)(((ch_tmp_byte & 0x03) | 0x80) | (szSrc[i] & 0x3e) >> 2);
            }
            else
            {
                strDest[i_cur_output] = '\0'; /* Terminate string */
                return 0; /* ERROR_INSUFFICIENT_BUFFER */
            }
        }
        else /* 3 byte utf8 representation */
        {
            if (i_cur_output + 3 < nDestLen)
            {
                strDest[i_cur_output++] = (char)(szSrc[i] >> 12 | 0xe0);
                strDest[i_cur_output++] = (char)(((szSrc[i] >> 6)  & 0x3f) | 0x80);
                strDest[i_cur_output++] = (char)((szSrc[i] & 0x3f) | 0x80);
            }
            else
            {
                strDest[i_cur_output] = '\0'; /* Terminate string */
                return 0; /* ERROR_INSUFFICIENT_BUFFER */
            }
        } /* @todo Handle surrogate pairs */
    }

    strDest[i_cur_output] = '\0'; /* Terminate string */

    return i_cur_output; /* This value is in bytes */
}


/**********************************************************************************/
/*                                                                                */
/* Function:     UTF8ToUnicode                                                    */
/* Description:  Convert UTF8 byte sequence to Unicode string                     */
/*                                                                                */
/* Parameters:   szSrc - UTF8 byte sequence to be converted                       */
/*                                 nSrcLen - Length of szSrc                      */
/*               strDest - unicode char buffer for                                */
/*               nDestLen - size (in characters) of buffer                        */
/*                                                                                */
/* Returns:      On success, the number of characters in the destination buffer   */
/*               0 on failure due to insufficient buffer size                     */
/*                                                                                */
/* History:      Created  {DATE]  {BY} {NAME} {PRODUCT REV}                       */
/*               Modified {DATE]  {BY} {NAME} {PRODUCT REV}                       */
/*                                                                                */
/**********************************************************************************/

OSCL_EXPORT_REF int32 oscl_UTF8ToUnicode(const char *szSrc, int32 nSrcLen, oscl_wchar *strDest, int32 nDestLen)
{
    int32 i = 0;
    int32 i_cur_output = 0;

    if (nDestLen <= 0)
    {
        // We cannot append terminate 0 at this case.
        return 0; /* ERROR_INSUFFICIENT_BUFFER */
    }

    unsigned char *pszSrc = (unsigned char *)szSrc;  /* cast to avoid signed/unsigned promomtion problems */
    while (i < nSrcLen)
    {
        if (SIGMASK_3_1 <= pszSrc[i]) /* 1st byte of 3 byte representation */
        {
            if (i + 2 < nSrcLen && i_cur_output + 1 < nDestLen)
            {
                strDest[i_cur_output++] = (wchar_t)(((wchar_t)pszSrc[i] << 12) |
                                                    (((wchar_t)pszSrc[i+1] & 0x3f) << 6) |
                                                    ((wchar_t)pszSrc[i+2] & 0x3f));
                i += 3;
            }
            else
            {
                strDest[i_cur_output] = 0; /* Terminate string */
                return 0; /* ERROR_INSUFFICIENT_BUFFER */
            }
        }
        else if (SIGMASK_2_1 <= pszSrc[i]) /* 1st byte of 2 byte representation */
        {
            if (i + 1 < nSrcLen && i_cur_output + 1 < nDestLen)
            {
                strDest[i_cur_output++] = (wchar_t)(((wchar_t)pszSrc[i] & ~0xc0) << 6 |
                                                    ((wchar_t)pszSrc[i+1] & ~0x80));
                i += 2;
            }
            else
            {
                strDest[i_cur_output] = 0; /* Terminate string */
                return 0; /* ERROR_INSUFFICIENT_BUFFER */
            }
        }
        else /* Single byte representation */
        {
            if (i < nSrcLen && i_cur_output + 1 < nDestLen)
            {
                strDest[i_cur_output++] = (wchar_t)pszSrc[i];
                ++i;
            }
            else
            {
                strDest[i_cur_output] = 0; /* Terminate string */
                return 0; /* ERROR_INSUFFICIENT_BUFFER */
            }
        }
    }

    strDest[i_cur_output] = 0; /* Terminate string */
    return i_cur_output;
}


