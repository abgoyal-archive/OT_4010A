
#ifndef __PVGSMAMR_H
#define __PVGSMAMR_H


// includes
#include <e32std.h>
#include <e32base.h>

#include "sp_dec.h"
#include "pvglobals.h"


// PVGsmDecoder AO
class CPVGsmDecoder : public CBase
{
    public:
        IMPORT_C static CPVGsmDecoder* NewL(void);
        IMPORT_C ~CPVGsmDecoder();
        IMPORT_C TInt StartL(void);

        // only port the API's used in PVPlayer 2.0
        IMPORT_C TInt DecodeFrame(enum Mode mode, unsigned char* compressedBlock, unsigned char* audioBuffer);
        IMPORT_C TInt InitDecoder(void);
        IMPORT_C void ExitDecoder(void);

    private:
        CPVGsmDecoder();
        void ConstructL(void);

        Speech_Decode_FrameState* decState;
        enum RXFrameType rx_type;
        struct globalDataStruct *gds;
};

#endif
