
#ifndef PV_2WAY_ENC_DATA_CHANNEL_DATAPATH_H_INCLUDED
#define PV_2WAY_ENC_DATA_CHANNEL_DATAPATH_H_INCLUDED

#ifndef PV_2WAY_DATA_CHANNEL_DATAPATH_H_INCLUDED
#include "pv_2way_data_channel_datapath.h"
#endif

class CPV2WayEncDataChannelDatapath : public CPV2WayDataChannelDatapath
{
    public:
        static CPV2WayEncDataChannelDatapath *NewL(PVLogger *aLogger,
                PVMFFormatType aFormat,
                CPV324m2Way *a2Way);

        ~CPV2WayEncDataChannelDatapath() {};

        void TSCPortClosed();
        PVMFPortInterface *GetTSCPort();
        PVMFPortInterface *GetSrcPort();

        CPV2WayPort *GetSourceInputPort()
        {
            return &iSourceInputPort;
        }
        void SetSourceInputPort(PVMFPortInterface *aPort);

        void SetFilePlayPort(PVMFPortInterface *aPort)
        {
            iFilePlayPort = aPort;
        }
        void UseFilePlayPort(bool aUseFilePlayPort);

    private:
        CPV2WayEncDataChannelDatapath(PVLogger *aLogger,
                                      PVMFFormatType aFormat,
                                      CPV324m2Way *a2Way) : CPV2WayDataChannelDatapath(aLogger, EEncodeDatapath, aFormat, a2Way),
                iFilePlayPort(NULL)
        {};
        TPV2WayNode* iNode;
        void OpenComplete();

        bool CheckPathSpecificOpen();
        bool PathSpecificClose();
        bool CheckPathSpecificStart();

        CPV2WayPort iSourceInputPort;

        PVMFPortInterface *iFilePlayPort;
        friend class CPV324m2Way;
};

#endif //PV_2WAY_ENC_DATA_CHANNEL_DATAPATH_H_INCLUDED


