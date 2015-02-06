

#ifndef M4V_PAYLOAD_INFO_H
#define M4V_PAYLOAD_INFO_H

#include "payload_info.h"

#define PVMF_SDP_DEFAULT_MPEG4_VIDEO_SAMPLE_RATE 90000

class M4vPayloadSpecificInfoType : public PayloadSpecificInfoTypeBase
{
    public:
        M4vPayloadSpecificInfoType(int payload)
        {
            payloadNumber = payload;

            frameWidth = -1;
            frameHeight = -1;
        };

        ~M4vPayloadSpecificInfoType()
        {};

        inline void setVOLHeader(OsclSharedPtr<uint8> HPtr)
        {
            m4v_VOLHeader = HPtr;
        };

        inline void setVOLHeaderSize(uint32 size)
        {
            m4v_VOLHeaderSize = size;
        };

        inline void setFrameWidth(int fWidth)
        {
            frameWidth = fWidth;
        };
        inline void setFrameHeight(int fHeight)
        {
            frameHeight = fHeight;
        };

        OsclSharedPtr<uint8> getVOLHeader()
        {
            return m4v_VOLHeader;
        };

        int getVOLLength()
        {
            return m4v_VOLHeaderSize;
        };

        inline int getFrameWidth()
        {
            return frameWidth;
        }

        inline int getFrameHeight()
        {
            return frameHeight;
        }

    private:
        OsclSharedPtr<uint8> m4v_VOLHeader;
        uint32 m4v_VOLHeaderSize;
        int frameWidth;
        int frameHeight;
};

#endif
