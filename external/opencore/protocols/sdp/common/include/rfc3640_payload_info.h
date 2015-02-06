

#ifndef RFC3640_PAYLOAD_INFO_H
#define RFC3640_PAYLOAD_INFO_H

#include "payload_info.h"

//#define PVMF_SDP_DEFAULT_RFC3640_VIDEO_SAMPLE_RATE 90000
//Default Values
#define AAC_HBR_SIZELENGTH_DEFAULT_VALUE        13
#define AAC_HBR_INDEXLENGTH_DEFAULT_VALUE       3
#define AAC_HBR_INDEXDELTALENGTH_DEFAULT_VALUE  3
#define AAC_HBR_CTSDELTALENGTH_DEFAULT_VALUE    0
#define AAC_HBR_DTSDELTALENGTH_DEFAULT_VALUE    0
#define AAC_HBR_HEADERSLENGTH_DEFAULT_VALUE     16
#define AAC_HBR_AUXDATASIZELENGTH_DEFAULT_VALUE 0

class RFC3640PayloadSpecificInfoType : public PayloadSpecificInfoTypeBase
{
    public:
        RFC3640PayloadSpecificInfoType(int payload)
        {
            payloadNumber = payload;
            //For now, just default to AAC high bit-rate.
            sizeLength             = 13;
            indexLength            = 3;
            indexDeltaLength       = 3;
            CTSDeltaLength         = 0;
            DTSDeltaLength         = 0;
            constantDuration       = 0;
            maxDisplacement        = 0;
            deInterleaveBufferSize = 0;
        };

        RFC3640PayloadSpecificInfoType()
        {};

        inline void setSizeLength(uint32 sLength)
        {
            sizeLength = sLength;
        };
        inline void setIndexLength(uint32 iLength)
        {
            indexLength = iLength;
        };
        inline void setIndexDeltaLength(uint32 idLength)
        {
            indexDeltaLength = idLength;
        };
        inline void setCTSDeltaLength(uint32 ctsdLength)
        {
            CTSDeltaLength = ctsdLength;
        };
        inline void setDTSDeltaLength(uint32 dtsLength)
        {
            DTSDeltaLength = dtsLength;
        };
        inline void setConstantDuration(uint32 cDuration)
        {
            constantDuration = cDuration;
        };
        inline void setMaxDisplacement(uint32 max)
        {
            maxDisplacement = max;
        };
        inline void setDeInterleaveBufferSize(uint32 size)
        {
            deInterleaveBufferSize = size;
        };

        inline uint32 getIndexLength()
        {
            return indexLength;
        };
        inline uint32 getSizeLength()
        {
            return sizeLength;
        };
        inline uint32 getIndexDeltaLength()
        {
            return indexDeltaLength;
        };
        inline uint32 getCTSDeltaLength()
        {
            return CTSDeltaLength;
        };
        inline uint32 getDTSDeltaLength()
        {
            return DTSDeltaLength;
        };
        inline uint32 getConstantDuration()
        {
            return constantDuration;
        };
        inline uint32 getMaxDisplacement()
        {
            return maxDisplacement;
        };
        inline uint32 getDeInterleaveBufferSize()
        {
            return deInterleaveBufferSize;
        };

    private:
        uint32 sizeLength;
        uint32 indexLength;
        uint32 indexDeltaLength;
        uint32 CTSDeltaLength;
        uint32 DTSDeltaLength;
        uint32 constantDuration;
        uint32 maxDisplacement;
        uint32 deInterleaveBufferSize;
};

#endif
