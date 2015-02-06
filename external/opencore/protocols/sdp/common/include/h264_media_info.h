
#ifndef H264_MEDIAINFO_H
#define H264_MEDIAINFO_H
#include "media_info.h"

class h264_mediaInfo : public mediaInfo
{
    public:
        h264_mediaInfo()
        {
            mediaInfo();
        };

        h264_mediaInfo(const h264_mediaInfo &pSource) : mediaInfo(pSource)
        {
        }
        ~h264_mediaInfo() {};
};

#endif
