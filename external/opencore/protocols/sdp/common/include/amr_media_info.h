

#ifndef AMR_MEDIAINFO_H
#define AMR_MEDIAINFO_H

#include "sdp_memory.h"
#include "media_info.h"

class amr_mediaInfo : public mediaInfo
{
    private:

        int maximumPTime;

        OSCL_HeapString<SDPParserAlloc> lang;
    public:
        amr_mediaInfo()
        {
            mediaInfo();
            maximumPTime = 0;
            lang = NULL;
        };

        amr_mediaInfo(const amr_mediaInfo &pSource) : mediaInfo(pSource)
        {

            setMaximumPTime(pSource.maximumPTime);
            setLang(pSource.lang);
        }

        ~amr_mediaInfo() {};

        inline bool setMaximumPTime(int mPTime)
        {
            maximumPTime = mPTime;
            return true;
        };
        inline void setLang(char* lan)
        {
            lang = lan;
        };
        inline void setLang(const OSCL_HeapString<SDPParserAlloc>& lan)
        {
            lang = lan;
        };
        inline void setLang(const OsclMemoryFragment memFrag)
        {
            lang.set((const char*)(memFrag.ptr), memFrag.len);
        };


        inline const char *getLang()
        {
            return lang.get_cstr();
        };
        inline int getMaximumPTime()
        {
            return maximumPTime;
        }


};

#endif
