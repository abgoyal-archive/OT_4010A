

#ifndef H263_PAYLOAD_INFO_H
#define H263_PAYLOAD_INFO_H

#include "payload_info.h"

#define PVMF_SDP_DEFAULT_H263_SAMPLE_RATE 90000

class H263PayloadSpecificInfoType : public PayloadSpecificInfoTypeBase
{
    public:
        H263PayloadSpecificInfoType(int payload)
        {
            payloadNumber = payload;

            h263_codecProfile = -1;
            h263_codecLevel = -1;
            frameWidth = -1;
            frameHeight = -1;
            hasFrameSizeField = false;
        };

        inline void setCodecProfile(int cProfile)
        {
            h263_codecProfile = cProfile;
        };

        inline void setCodecLevel(int cLevel)
        {
            h263_codecLevel = cLevel;
        };

        inline void setFrameWidth(int fWidth)
        {
            frameWidth = fWidth;
        };
        inline void setFrameHeight(int fHeight)
        {
            frameHeight = fHeight;
        };

        int getCodecProfile()
        {
            return h263_codecProfile;
        };

        int getCodecLevel()
        {
            return h263_codecLevel;
        };

        inline int getFrameWidth()
        {
            return frameWidth;
        }

        inline int getFrameHeight()
        {
            return frameHeight;
        }

        inline void setHasFrameSizeField(bool value)
        {
            hasFrameSizeField = value;
    	 }

        inline bool getHasFrameSizeField()
        {
            return hasFrameSizeField;
        }

    private:
        int h263_codecProfile;
        int h263_codecLevel;
        int frameWidth;
        int frameHeight;
        bool hasFrameSizeField;
};


#endif
