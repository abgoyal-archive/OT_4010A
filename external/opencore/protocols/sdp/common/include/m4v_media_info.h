

#ifndef M4V_MEDIAINFO_H
#define M4V_MEDIAINFO_H
#include "media_info.h"


class m4v_mediaInfo : public mediaInfo
{
    private:

        double frameRate;
        int iFrameInterval;

    public:
        m4v_mediaInfo()
        {
            mediaInfo();
            frameRate = 0.0;
            iFrameInterval = -1;

        };

        m4v_mediaInfo(const m4v_mediaInfo &pSource) : mediaInfo(pSource)
        {

            setFrameRate(pSource.frameRate);
            setIFrameInterval(pSource.iFrameInterval);
        }
        const m4v_mediaInfo & operator=(const m4v_mediaInfo &pSource)
        {
            if (this != &pSource)
            {
                ((mediaInfo&) *this) = pSource;

                setFrameRate(pSource.frameRate);
                setIFrameInterval(pSource.iFrameInterval);
            }
            return *this;
        }

        ~m4v_mediaInfo() {};

        inline void setFrameRate(double fRate)
        {
            frameRate = fRate;
        };
        inline void setIFrameInterval(int IFI)
        {
            iFrameInterval = IFI;
        };

        inline double getFrameRate()
        {
            return frameRate;
        };
        inline int getIFrameInterval()
        {
            return iFrameInterval;
        };


};

#endif
