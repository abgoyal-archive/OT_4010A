

#ifndef H263_MEDIAINFO_H
#define H263_MEDIAINFO_H
#include "media_info.h"

class h263_mediaInfo : public mediaInfo
{
    private:

    public:
        h263_mediaInfo()
        {
            mediaInfo();

        };

        h263_mediaInfo(const h263_mediaInfo &pSource) : mediaInfo(pSource)
        {

        }

        ~h263_mediaInfo() {};


};

#endif
