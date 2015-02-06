

#ifndef PVMP3_DECODER_H
#define PVMP3_DECODER_H


#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#include "pvmp3decoder_api.h"
#include "oscl_base.h"




#define KMP3_MAX_OUTPUT_SIZE    4608



// CPvMP3_Decoder



// CPvMP3_Decoder
class CPvMP3_Decoder
{
    public:
        OSCL_IMPORT_REF void ConstructL();
        OSCL_IMPORT_REF static CPvMP3_Decoder *NewL();
        OSCL_IMPORT_REF ~CPvMP3_Decoder();

        OSCL_IMPORT_REF int32 StartL(tPVMP3DecoderExternal * pExt,
                                     bool aAllocateInputBuffer = false,
                                     bool aAllocateOutputBuffer = false,
                                     bool crcEnabler = false,
                                     e_equalization equalizType = flat
                                    );
        OSCL_IMPORT_REF int32 ExecuteL(tPVMP3DecoderExternal * pExt);
        OSCL_IMPORT_REF void ResetDecoderL();
        OSCL_IMPORT_REF void StopL();

        OSCL_IMPORT_REF void TerminateDecoderL();
        OSCL_IMPORT_REF int32 SeekMp3Synchronization(tPVMP3DecoderExternal * pExt);



    private:
        bool iAllocateInputBuffer;
        bool iAllocateOutputBuffer;

        uint8 *pMem;

        uint8* iInputBuf;
        int16* iOutputBuf;
};



#endif

