
#ifndef SRP_PORTS_H_INCLUDED
#define SRP_PORTS_H_INCLUDED

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef PVMF_PORT_BASE_IMPL_H_INCLUDED
#include "pvmf_port_base_impl.h"
#endif

#ifndef SRP_H
#include "srp.h"
#endif

#include "media_packet.h"

class SRPLowerLayerPortOut : public PvmfPortBaseImpl
{
        // All requests are synchronous
    public:
        SRPLowerLayerPortOut() : PvmfPortBaseImpl(SRP_OUTPUT_PORT_TAG, NULL) {};

        ~SRPLowerLayerPortOut() {}
};

class SRPLowerLayerPortIn : public PvmfPortBaseImpl
{
        // All requests are synchronous
    public:
        SRPLowerLayerPortIn(SRP *aSrp) : PvmfPortBaseImpl(SRP_INPUT_PORT_TAG, NULL),
                iSrp(aSrp)
        {};

        ~SRPLowerLayerPortIn() {};

        virtual PVMFStatus Receive(PVMFSharedMediaMsgPtr aMsg)
        {
            PVMFSharedMediaDataPtr mediaData;
            convertToPVMFMediaData(mediaData, aMsg);

            // send packet to SRP
            iSrp->LowerLayerRx(mediaData);

            return PVMFSuccess;
        }

    private:
        SRP *iSrp;
};

class SRPUpperLayerPortOut : public PvmfPortBaseImpl
{
        // All requests are synchronous
    public:
        SRPUpperLayerPortOut() : PvmfPortBaseImpl(SRP_OUTPUT_PORT_TAG, NULL) {};

        ~SRPUpperLayerPortOut() {}
};

class SRPUpperLayerPortIn : public PvmfPortBaseImpl
{
        // All requests are synchronous
    public:
        SRPUpperLayerPortIn(SRP *aSrp) : PvmfPortBaseImpl(SRP_INPUT_PORT_TAG, NULL),
                iSrp(aSrp)
        {};

        ~SRPUpperLayerPortIn() {};

        virtual PVMFStatus Receive(PVMFSharedMediaMsgPtr aMsg)
        {
            PVMFSharedMediaDataPtr mediaData;
            convertToPVMFMediaData(mediaData, aMsg);

            // send packet to SRP
            iSrp->UpperLayerRx(mediaData);

            return PVMFSuccess;
        }

    private:
        SRP *iSrp;
};

#endif
