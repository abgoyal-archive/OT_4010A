
#ifndef PV_2WAY_DEC_DATA_CHANNEL_DATAPATH_H_INCLUDED
#define PV_2WAY_DEC_DATA_CHANNEL_DATAPATH_H_INCLUDED

#ifndef PV_2WAY_DATA_CHANNEL_DATAPATH_H_INCLUDED
#include "pv_2way_data_channel_datapath.h"
#endif


class CPV2WayDecDataChannelDatapath : public CPV2WayDataChannelDatapath
{
    public:
        static CPV2WayDecDataChannelDatapath *NewL(PVLogger *aLogger,
                PVMFFormatType aFormat,
                CPV324m2Way *a2Way);

        ~CPV2WayDecDataChannelDatapath() {};

        void TSCPortClosed();
        PVMFPortInterface *GetTSCPort();
        void SkipComplete(TPV2WayNode* aNode);
        bool IsSkipComplete() const;
    private:
        CPV2WayDecDataChannelDatapath(PVLogger *aLogger,
                                      PVMFFormatType aFormat,
                                      CPV324m2Way *a2Way) : CPV2WayDataChannelDatapath(aLogger, EDecodeDatapath, aFormat, a2Way), iSkipComplete(false)
        {};
        TPV2WayNode* iNode;
        void OpenComplete();
        bool iSkipComplete;
};

#endif //PV_2WAY_DEC_DATA_CHANNEL_DATAPATH_H_INCLUDED


