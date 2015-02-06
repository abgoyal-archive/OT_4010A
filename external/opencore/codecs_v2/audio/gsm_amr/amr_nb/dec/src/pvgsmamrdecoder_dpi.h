

#ifndef __PVGSMAMRDECODER_H
#define __PVGSMAMRDECODER_H

#include "oscl_base.h"

#include "gsmamr_dec.h"
#include "gsmamrdpidecoder.h"

// PVGSMAMRDecoder
class CPVGSMAMRDecoder
{
    public:
        OSCL_IMPORT_REF CPVGSMAMRDecoder();
        OSCL_IMPORT_REF ~CPVGSMAMRDecoder();

        OSCL_IMPORT_REF int32 InitDecoder(void);
        OSCL_IMPORT_REF int32 DecodeFrame(Frame_Type_3GPP aType,
                                          uint8* aCompressedBlock,
                                          uint8* aAudioBuffer,
                                          int32 aFormat);
        OSCL_IMPORT_REF int32 ResetDecoder(void);
        OSCL_IMPORT_REF void TerminateDecoder(void);

    private:
        void* iDecState;
        CPvGsmAmrDPIDecoder *iDecoder;
};

#endif

