

#ifndef AMR_PAYLOAD_INFO_H
#define AMR_PAYLOAD_INFO_H

#include "payload_info.h"

#define PVMF_SDP_DEFAULT_AMR_SAMPLE_RATE 8000

class AmrPayloadSpecificInfoType : public PayloadSpecificInfoTypeBase
{
    public:
        AmrPayloadSpecificInfoType(int payload)
        {
            payloadNumber = payload;

            amr_interLeaving  = -1;
            amr_maximumPTime = -1;
            amr_modeChangePeriod = -1;
            amr_codecModeList = 0;
            amr_modeChangeNeighbor = false;
            amr_CRC = false;
            amr_octetAlign = false;
            amr_robustSorting = false;
        };

        ~AmrPayloadSpecificInfoType()
        {};

        inline void setCodecModeList(int cModeList)
        {
            amr_codecModeList = cModeList;
        };

        inline void setModeChangePeriod(int mChangePeriod)
        {
            amr_modeChangePeriod = mChangePeriod;
        };
        inline void setModeChangeNeighbor(bool mcneighbor)
        {
            amr_modeChangeNeighbor = mcneighbor;
        };

        inline void setMaximumPTime(int mPTime)
        {
            amr_maximumPTime = mPTime;
        };

        inline void setRobustSorting(bool arg)
        {
            amr_robustSorting = arg;
        };
        inline void setOctetAlign(bool arg)
        {
            amr_octetAlign = arg;
        };
        inline void setCRC(bool arg)
        {
            amr_CRC = arg;
        };
        inline void setInterLeaving(int iLeaving)
        {
            amr_interLeaving = iLeaving;
        };

        inline int getMaximumPTime()
        {
            return amr_maximumPTime;
        };

        bool getOctetAlign()
        {
            return amr_octetAlign;
        };

        inline int getCodecModeList()
        {
            return amr_codecModeList;
        };

        inline int getModeChangePeriod()
        {
            return amr_modeChangePeriod;
        };

        inline bool getModeChangeNeighbor()
        {
            return amr_modeChangeNeighbor;
        };

        inline bool getRobustSorting()
        {
            return amr_robustSorting;
        };

        inline int getInterleaving()
        {
            return amr_interLeaving;
        };

        inline bool getCrc()
        {
            return amr_CRC;
        };


    private:
        int     amr_codecModeList;
        int     amr_modeChangePeriod;
        bool    amr_modeChangeNeighbor;
        int     amr_maximumPTime;
        bool    amr_robustSorting;
        bool    amr_octetAlign;
        bool    amr_CRC;
        int     amr_interLeaving;
};

#endif
