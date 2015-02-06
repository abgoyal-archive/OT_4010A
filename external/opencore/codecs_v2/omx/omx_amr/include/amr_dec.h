
#ifndef AMR_DEC_H_INCLUDED
#define AMR_DEC_H_INCLUDED

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OMX_Component_h
#include "OMX_Component.h"
#endif

#ifndef FRAME_TYPE_3GPP_H
#include "frame_type_3gpp.h"
#endif

#ifndef _DECODER_AMR_NB_H
#include "decoder_gsm_amr.h"
#endif

#ifndef _DECODER_AMR_WB_H
#include "decoder_amr_wb.h"
#endif



class OmxAmrDecoder
{
    public:
        OmxAmrDecoder();

        OMX_BOOL AmrDecInit(OMX_AUDIO_AMRFRAMEFORMATTYPE aInFormat, OMX_AUDIO_AMRBANDMODETYPE aInMode);

        void AmrDecDeinit();

        OMX_BOOL AmrDecodeFrame(OMX_S16* aOutputBuffer,
                                OMX_U32* aOutputLength, OMX_U8** aInBuffer,
                                OMX_U32* aInBufSize, OMX_S32* aIsFirstBuffer);

        OMX_BOOL AmrDecodeSilenceFrame(OMX_S16* aOutputBuffer,
                                       OMX_U32* aOutputLength);


        void ResetDecoder(); // for repositioning

        OMX_S32 iAmrInitFlag;

    private:

        Frame_Type_3GPP GetFrameTypeLength(OMX_U8* &ptr, OMX_S32 *pLength);
        void GetStartPointsForIETFCombinedMode(OMX_U8* aPtrIn, OMX_U32 aLength,
                                               OMX_U8* &aTocPtr, OMX_S32* aNumOfBytes);

        OMX_S16 iOmxInputFormat;
        OMX_U8  *iTocTablePtr;
        OMX_S32 iAMRFramesinTOC;


        //CDecoder_AMR_NB* iAudioAmrDecoder;
        CDecoder_AMRInterface* iAudioAmrDecoder;
        tPVAmrDecoderExternal* iCodecExternals;

        OMX_U32 iOutputFrameSize;
        OMX_BOOL iNarrowBandFlag;

        OMX_S16 iDecHomingFlag;
        OMX_S16 iDecHomingFlagOld;

		unsigned char m_BSbuf[2048];
		unsigned char *m_tmpBSbuf;
		int  m_BSbufLen;
		int  m_ResidualSize;
		bool m_bBuffer_Flag;
		bool m_bDone_Flag;

};



#endif  //#ifndef AMR_DEC_H_INCLUDED

