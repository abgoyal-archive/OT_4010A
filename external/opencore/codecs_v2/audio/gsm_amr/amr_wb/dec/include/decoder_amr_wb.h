
//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
//  File: decoder_amr_wb.h                                                      //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#ifndef _DECODER_AMR_WB_H
#define _DECODER_AMR_WB_H

#include "oscl_base.h"
#include "pvamrwbdecoder_api.h"
#include "pvgsmamrdecoderinterface.h"

// CDecoder_AMR_WB
class CDecoder_AMRInterface;
class CDecoder_AMR_WB: public CDecoder_AMRInterface
{
    public:
        OSCL_IMPORT_REF void ConstructL();
        OSCL_IMPORT_REF static CDecoder_AMR_WB *NewL();
        OSCL_IMPORT_REF virtual ~CDecoder_AMR_WB();

        OSCL_IMPORT_REF virtual int32 StartL(tPVAmrDecoderExternal * pExt,
                                             bool aAllocateInputBuffer  = false,
                                             bool aAllocateOutputBuffer = false);

        OSCL_IMPORT_REF virtual int32 ExecuteL(tPVAmrDecoderExternal * pExt);

        OSCL_IMPORT_REF virtual int32 ResetDecoderL(void);
        OSCL_IMPORT_REF virtual void StopL();
        OSCL_IMPORT_REF virtual void TerminateDecoderL();

    private:
        void *st;                   /*   State structure  */
        void *pt_st;
        int16 *ScratchMem;

        uint8* iInputBuf;
        int16* iInputSampleBuf;
        int16* iOutputBuf;


};


#endif

