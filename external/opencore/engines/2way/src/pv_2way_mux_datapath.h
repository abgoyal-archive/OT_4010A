
#ifndef PV_2WAY_MUX_DATAPATH_H_INCLUDED
#define PV_2WAY_MUX_DATAPATH_H_INCLUDED


#ifndef PV_2WAY_DATAPATH_H_INCLUDED
#include "pv_2way_datapath.h"
#endif

class CPV2WayMuxDatapath : public CPV2WayDatapath
{
    public:
        static CPV2WayMuxDatapath *NewL(PVLogger *aLogger,
                                        PVMFFormatType aFormat,
                                        CPV324m2Way *a2Way);

        virtual ~CPV2WayMuxDatapath() {};
        virtual bool SingleNodeOpen()
        {
            return (i2Way->iTerminalType == PV_SIP);
        };

    private:
        CPV2WayMuxDatapath(PVLogger *aLogger,
                           PVMFFormatType aFormat,
                           CPV324m2Way *a2Way) : CPV2WayDatapath(aLogger, EMuxDatapath, aFormat, a2Way)
        {};

        void OpenComplete();
        void PauseComplete();
        void ResumeComplete();
        void CloseComplete();

        void DatapathError();
};


#endif //PV_2WAY_MUX_DATAPATH_H_INCLUDED


