

#ifndef OMX_AMRENC_COMPONENT_H_INCLUDED
#define OMX_AMRENC_COMPONENT_H_INCLUDED

#ifndef PV_OMXCOMPONENT_H_INCLUDED
#include "pv_omxcomponent.h"
#endif

#ifndef AMR_DEC_H_INCLUDED
#include "amr_enc.h"
#endif


#define INPUT_BUFFER_SIZE_AMR_ENC (320 * MAX_NUM_OUTPUT_FRAMES_PER_BUFFER)  //3200

#define OUTPUT_BUFFER_SIZE_AMR_ENC (MAX_NUM_OUTPUT_FRAMES_PER_BUFFER * MAX_AMR_FRAME_SIZE)  //10 * 32 = 320

#define NUMBER_INPUT_BUFFER_AMR_ENC  5
#define NUMBER_OUTPUT_BUFFER_AMR_ENC  2


#define omx_min(a, b)  ((a) <= (b) ? (a) : (b));

class OmxComponentAmrEncoderAO : public OmxComponentAudio
{
    public:

        OmxComponentAmrEncoderAO();
        ~OmxComponentAmrEncoderAO();


        OMX_ERRORTYPE ConstructComponent(OMX_PTR pAppData, OMX_PTR pProxy);
        OMX_ERRORTYPE DestroyComponent();

        OMX_ERRORTYPE ComponentInit();
        OMX_ERRORTYPE ComponentDeInit();

        static void ComponentGetRolesOfComponent(OMX_STRING* aRoleString);

        void ProcessData();

        void ProcessInBufferFlag();
        void SyncWithInputTimestamp();

    private:

        OMX_U32         iInputFrameLength;
        OMX_U32         iMaxNumberOutputFrames;
        OMX_U32         iActualNumberOutputFrames;

        OMX_TICKS       iCurrentTimestamp;
        OmxAmrEncoder*  ipAmrEnc;
};

#endif // OMX_AMRENC_COMPONENT_H_INCLUDED
