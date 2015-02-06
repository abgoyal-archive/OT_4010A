
//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
//  File: decoder_amr_nb.h                                                      //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#ifndef _DECODER_AMR_NB_H
#define _DECODER_AMR_NB_H

#include "oscl_base.h"
#include "pvgsmamrdecoderinterface.h"

// CDecoder_AMR_WB
class CDecoder_AMRInterface;
class CDecoder_AMR_NB: public CDecoder_AMRInterface
{
    public:
        OSCL_IMPORT_REF void ConstructL();
        OSCL_IMPORT_REF static CDecoder_AMR_NB *NewL();
        OSCL_IMPORT_REF virtual ~CDecoder_AMR_NB();

        OSCL_IMPORT_REF virtual int32 StartL(tPVAmrDecoderExternal * pExt,
                                             bool aAllocateInputBuffer  = false,
                                             bool aAllocateOutputBuffer = false);

        OSCL_IMPORT_REF virtual int32 ExecuteL(tPVAmrDecoderExternal * pExt);

        OSCL_IMPORT_REF virtual int32 ResetDecoderL();
        OSCL_IMPORT_REF virtual void StopL();
        OSCL_IMPORT_REF virtual void TerminateDecoderL();

    private:
        void* iDecState;

        int16* iInputBuf;
        int16* iOutputBuf;


};


#endif

