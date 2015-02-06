

#ifndef STILL_IMAGE_MEDIAINFO_H
#define STILL_IMAGE_MEDIAINFO_H

#include "sdp_memory.h"
#include "media_info.h"

class still_image_mediaInfo : public mediaInfo
{
    private:
        int bitRate;
        OSCL_HeapString<SDPParserAlloc> payload;
    public:
        still_image_mediaInfo()
        {
            mediaInfo();
            setMIMEType("X-MP4V-IMAGE");
            bitRate = -1;
            payload = NULL;
            totalStringLength = oscl_strlen("m=application 0 RTSP/GET_PARAMETER IMAGE");
        };

        still_image_mediaInfo(const still_image_mediaInfo &pSource) : mediaInfo(pSource)
        {
            setBitRate(pSource.bitRate);
            setPayload(pSource.payload);
        }

        ~still_image_mediaInfo() {};

        inline void setBitRate(int bRate)
        {
            bitRate = bRate;
        };
        inline void setPayload(char* pload)
        {
            payload = pload;
        };
        inline void setPayload(OSCL_HeapString<SDPParserAlloc> pload)
        {
            payload = pload;
        };

        inline int getBitRate()
        {
            return bitRate;
        };
        inline char* getPayload()
        {
            return (char *)payload.get_cstr();
        };
};

#endif
