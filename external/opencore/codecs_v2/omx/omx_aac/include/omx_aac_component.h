

#ifndef OMX_AAC_COMPONENT_H_INCLUDED
#define OMX_AAC_COMPONENT_H_INCLUDED

#ifndef PV_OMXCOMPONENT_H_INCLUDED
#include "pv_omxcomponent.h"
#endif

#ifndef AAC_DEC_H_INCLUDED
#include "aac_dec.h"
#endif

#ifndef AAC_TIMESTAMP_H_INCLUDED
#include "aac_timestamp.h"
#endif


#define INPUT_BUFFER_SIZE_AAC 1536
#define OUTPUT_BUFFER_SIZE_AAC 8192

#define NUMBER_INPUT_BUFFER_AAC  10
#define NUMBER_OUTPUT_BUFFER_AAC  9


class OpenmaxAacAO : public OmxComponentAudio
{
    public:

        OpenmaxAacAO();
        ~OpenmaxAacAO();

        OMX_ERRORTYPE ConstructComponent(OMX_PTR pAppData, OMX_PTR pProxy);
        OMX_ERRORTYPE DestroyComponent();

        OMX_ERRORTYPE ComponentInit();
        OMX_ERRORTYPE ComponentDeInit();

        static void ComponentGetRolesOfComponent(OMX_STRING* aRoleString);
        void ProcessData();
        void SyncWithInputTimestamp();
        void ProcessInBufferFlag();

        void UpdateAACPlusFlag(OMX_BOOL aAacPlusFlag);
        void ResetComponent();
        OMX_ERRORTYPE GetConfig(
            OMX_IN  OMX_HANDLETYPE hComponent,
            OMX_IN  OMX_INDEXTYPE nIndex,
            OMX_INOUT OMX_PTR pComponentConfigStructure);


    private:

        void CheckForSilenceInsertion();
        void DoSilenceInsertion();


        AacTimeStampCalc iCurrentFrameTS;
        OmxAacDecoder* ipAacDec;
        
        int resetFlag;					//by HP Cheng to mute first few frames and ramp up after decoder reset (after seek)
};



#endif // OMX_AAC_COMPONENT_H_INCLUDED
