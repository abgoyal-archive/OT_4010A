

#ifndef RFC3640_MEDIAINFO_H_INCLUDED
#define RFC3640_MEDIAINFO_H_INCLUDED

#ifndef MEDIAINFO_H
#include "media_info.h"
#endif

class rfc3640_mediaInfo : public mediaInfo
{
    private:

        uint32 streamType;
        OSCL_HeapString<SDPParserAlloc> mode;

    public:
        //For now, just default to AAC high bit-rate.
        rfc3640_mediaInfo()
        {
            mediaInfo();
            mode = NULL;

        };

        rfc3640_mediaInfo(const rfc3640_mediaInfo &pSource) : mediaInfo(pSource)
        {

        }
        const rfc3640_mediaInfo & operator=(const rfc3640_mediaInfo &pSource)
        {
            if (this != &pSource)
            {
                ((mediaInfo&) *this) = pSource;
                setStreamType(pSource.streamType);
                setModeType(pSource.mode);
            }
            return *this;
        }

        ~rfc3640_mediaInfo() {};

        inline void setStreamType(uint32 stype)
        {
            streamType = stype;
        };

        inline uint32 getStreamType()
        {
            return streamType;
        };
        inline void setMode(char* md)
        {
            mode = md;
        };
        inline void setModeType(const OSCL_HeapString<SDPParserAlloc>& md)
        {
            mode = md;
        };
        inline void setModeType(const OsclMemoryFragment memFrag)
        {
            mode.set((const char*)(memFrag.ptr), memFrag.len);
        };
        inline char* getMode(void)
        {
            return mode.get_str();
        };



};

#endif
