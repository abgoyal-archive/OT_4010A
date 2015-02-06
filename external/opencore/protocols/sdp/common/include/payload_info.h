

#ifndef PAYLOAD_INFO_H
#define PAYLOAD_INFO_H

class PayloadSpecificInfoTypeBase
{
    public:
        PayloadSpecificInfoTypeBase()
        {
            payloadNumber = 0;
            sampleRate = 0;
            noOfChannels = 0;
            maxBufferSize = 0;
            profileLevelID = -1;
            configSize = 0;
        };

        virtual ~PayloadSpecificInfoTypeBase()
        {};

        inline void setPayloadNumber(int pNumber)
        {
            payloadNumber = pNumber;
        };

        inline void setSampleRate(int sRate)
        {
            sampleRate = sRate;
        };

        inline void setNoOfChannels(int channels)
        {
            noOfChannels = channels;
        };

        inline void setMaxBufferSize(int maxBuff)
        {
            maxBufferSize = maxBuff;
        };

        inline void setProfileLevelID(int pID)
        {
            profileLevelID = pID;
        };

        inline void setDecoderSpecificInfo(OsclSharedPtr<uint8> Info)
        {
            configHeader = Info;
        };

        OsclSharedPtr<uint8> getConfigHeader()
        {
            return configHeader;
        }

        inline void setDecoderSpecificInfoSize(uint32 size)
        {
            configSize = size;
        };

        uint32 getConfigSize()
        {
            return configSize;
        };
        uint32 getPayloadNumber()
        {
            return payloadNumber;
        };

        uint32 getSampleRate()
        {
            return sampleRate;
        };

        uint32 getNumberOfChannels()
        {
            return noOfChannels;
        };

        inline int getMaxBufferSize()
        {
            return maxBufferSize;
        };

        inline int getProfileLevelID()
        {
            return profileLevelID;
        };

        uint32  payloadNumber;
        uint32  sampleRate;
        uint32  noOfChannels;
        int     maxBufferSize;
        int     profileLevelID;
        OsclSharedPtr<uint8> configHeader;
        uint32  configSize;

};

#endif
