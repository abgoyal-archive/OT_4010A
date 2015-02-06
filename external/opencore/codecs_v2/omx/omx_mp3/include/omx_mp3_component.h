

#ifndef OMX_MP3_COMPONENT_H_INCLUDED
#define OMX_MP3_COMPONENT_H_INCLUDED

#ifndef PV_OMXCOMPONENT_H_INCLUDED
#include "pv_omxcomponent.h"
#endif

#ifndef MP3_DEC_H_INCLUDED
#include "mp3_dec.h"
#endif


#ifndef MP3_TIMESTAMP_H_INCLUDED
#include "mp3_timestamp.h"
#endif



#define INPUT_BUFFER_SIZE_MP3 8192
#define OUTPUT_BUFFER_SIZE_MP3 4608

#define NUMBER_INPUT_BUFFER_MP3  10
#define NUMBER_OUTPUT_BUFFER_MP3  9


class OpenmaxMp3AO : public OmxComponentAudio
{
    public:

        OpenmaxMp3AO();
        ~OpenmaxMp3AO();

        OMX_ERRORTYPE ConstructComponent(OMX_PTR pAppData, OMX_PTR pProxy);
        OMX_ERRORTYPE DestroyComponent();

        OMX_ERRORTYPE ComponentInit();
        OMX_ERRORTYPE ComponentDeInit();

        static void ComponentGetRolesOfComponent(OMX_STRING* aRoleString);
        void ProcessData();
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

        Mp3Decoder*      ipMp3Dec;
        Mp3TimeStampCalc iCurrentFrameTS;


};

#endif // OMX_MP3_COMPONENT_H_INCLUDED
