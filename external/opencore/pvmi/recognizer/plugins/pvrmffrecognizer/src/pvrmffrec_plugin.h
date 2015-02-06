
#ifndef PVRMFFREC_PLUGIN_H_INCLUDED
#define PVRMFFREC_PLUGIN_H_INCLUDED

#ifndef PVMF_RECOGNIZER_PLUGIN_H_INCLUDED
#include "pvmf_recognizer_plugin.h"
#endif

class PVRMFFRecognizerPlugin : public PVMFRecognizerPluginInterface
{
    public:
        PVRMFFRecognizerPlugin()
        {
        };

        ~PVRMFFRecognizerPlugin()
        {
        };

        PVMFStatus SupportedFormats(PVMFRecognizerMIMEStringList& aSupportedFormatsList);

        PVMFStatus Recognize(PVMFDataStreamFactory& aSourceDataStreamFactory, PVMFRecognizerMIMEStringList* aFormatHint,
                             Oscl_Vector<PVMFRecognizerResult, OsclMemAllocator>& aRecognizerResult);

        PVMFStatus GetRequiredMinBytesForRecognition(uint32& aBytes);
};

#endif // PVRMFFREC_PLUGIN_H_INCLUDED

