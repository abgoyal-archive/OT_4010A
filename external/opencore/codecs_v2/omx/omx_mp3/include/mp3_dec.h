
#ifndef MP3_DEC_H_INCLUDED
#define MP3_DEC_H_INCLUDED

#ifndef OMX_Component_h
#include "OMX_Component.h"
#endif


#ifndef PVMP3_DECODER_H
#include "pvmp3_decoder.h"
#endif
#ifdef MP3_DECLIB_PROPRIETARY

#include "mp3dec_exp.h"
#include "mcps.h"
#endif


class Mp3Decoder
{
    public:

        Mp3Decoder();

        OMX_BOOL Mp3DecInit(OMX_AUDIO_CONFIG_EQUALIZERTYPE* aEqualizerType);
        void Mp3DecDeinit();

        Int Mp3DecodeAudio(OMX_S16* aOutBuff,
                           OMX_U32* aOutputLength, OMX_U8** aInputBuf,
                           OMX_U32* aInBufSize,
                           OMX_S32* aFrameCount,
                           OMX_AUDIO_PARAM_PCMMODETYPE* aAudioPcmParam,
                           OMX_AUDIO_PARAM_MP3TYPE* aAudioMp3Param,
                           OMX_BOOL aMarkerFlag,
#ifdef MP3_DECLIB_PROPRIETARY

                           OMX_BOOL* aResizeFlag,
                           OMX_BOOL iEndofStream);
		void RampUp(OMX_S16* aRampBuff,
							OMX_U32 aHalfRampSample);
#else
                           OMX_BOOL* aResizeFlag);
#endif

        void ResetDecoder(); // for repositioning

        OMX_S32 iInputUsedLength;
        OMX_S32 iInitFlag;
#ifdef MP3_DECLIB_PROPRIETARY
        Mp3dec_handle* mp3Handle;
        int32 working_BUF_size1, working_BUF_size2, minBS_Size, PCM_size, BS_BUF_size;
        void *pWorking_BUF1, *pWorking_BUF2, *pBS_BUF, *pPCM_BUF;
		OMX_U8 *pPreBS_BUF;
        OMX_U8 *pRead, *pWrite;
        int size, validBSLen, size_PreBS_BUF, size_PreBUF, size_InBUF;
        MCPSinfo mcpsInfo;
		OMX_S8 iPreBufferPrepareFlag;
		OMX_S8 iPreBufferEnableFlag;
        OMX_U32 aFlagCount;

		OMX_U8 aFlagFrameCountAfterReset;
#endif

    private:

        CPvMP3_Decoder* iAudioMp3Decoder;
        tPVMP3DecoderExternal* iMP3DecExt;

};



#endif  //#ifndef MP3_DEC_H_INCLUDED

