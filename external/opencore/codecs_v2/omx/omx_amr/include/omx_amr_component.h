

#ifndef OMX_AMR_COMPONENT_H_INCLUDED
#define OMX_AMR_COMPONENT_H_INCLUDED

#ifndef PV_OMXCOMPONENT_H_INCLUDED
#include "pv_omxcomponent.h"
#endif

#ifndef AMR_DEC_H_INCLUDED
#include "amr_dec.h"
#endif

#define INPUT_BUFFER_SIZE_AMR 2000
#define OUTPUT_BUFFER_SIZE_AMR 640

#define NUMBER_INPUT_BUFFER_AMR  10
#define NUMBER_OUTPUT_BUFFER_AMR  9

// we are assuming 16 bits per sample
#define AMR_NB_OUTPUT_FRAME_SIZE_IN_BYTES 320
#define AMR_WB_OUTPUT_FRAME_SIZE_IN_BYTES 640

class OpenmaxAmrAO : public OmxComponentAudio
{
    public:

        OpenmaxAmrAO();
        ~OpenmaxAmrAO();


        OMX_ERRORTYPE ConstructComponent(OMX_PTR pAppData, OMX_PTR pProxy);
        OMX_ERRORTYPE DestroyComponent();

        OMX_ERRORTYPE ComponentInit();
        OMX_ERRORTYPE ComponentDeInit();

        static void ComponentGetRolesOfComponent(OMX_STRING* aRoleString);
        void ProcessData();

        void DecodeWithoutMarker();
        void DecodeWithMarker();

        void ComponentBufferMgmtWithoutMarker();
        void SyncWithInputTimestamp();
        void ProcessInBufferFlag();

        void ResetComponent();

        OMX_ERRORTYPE GetConfig(
            OMX_IN  OMX_HANDLETYPE hComponent,
            OMX_IN  OMX_INDEXTYPE nIndex,
            OMX_INOUT OMX_PTR pComponentConfigStructure);

    private:

        void CheckForSilenceInsertion();
        void DoSilenceInsertion();

        OMX_TICKS               iCurrentTimestamp;
        OMX_S32                 iPreviousFrameLength;
        OMX_U32                 iZeroFramesNeeded;

        OmxAmrDecoder* ipAmrDec;
};

#endif // OMX_AMR_COMPONENT_H_INCLUDED
