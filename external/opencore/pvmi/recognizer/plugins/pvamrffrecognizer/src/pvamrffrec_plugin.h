
#ifndef PVAMRFFREC_PLUGIN_H_INCLUDED
#define PVAMRFFREC_PLUGIN_H_INCLUDED

#ifndef PVMF_RECOGNIZER_PLUGIN_H_INCLUDED
#include "pvmf_recognizer_plugin.h"
#endif

#define AMRFF_MIN_DATA_SIZE_FOR_RECOGNITION 5

class PVAMRFFRecognizerPlugin : public PVMFRecognizerPluginInterface
{
    public:
        PVAMRFFRecognizerPlugin()
        {
        };

        ~PVAMRFFRecognizerPlugin()
        {
        };

        PVMFStatus SupportedFormats(PVMFRecognizerMIMEStringList& aSupportedFormatsList);

        PVMFStatus Recognize(PVMFDataStreamFactory& aSourceDataStreamFactory,
                             PVMFRecognizerMIMEStringList* aFormatHint,
                             Oscl_Vector<PVMFRecognizerResult, OsclMemAllocator>& aRecognizerResult);

        PVMFStatus GetRequiredMinBytesForRecognition(uint32& aBytes);
};

#endif // PVAMRFFREC_PLUGIN_H_INCLUDED

