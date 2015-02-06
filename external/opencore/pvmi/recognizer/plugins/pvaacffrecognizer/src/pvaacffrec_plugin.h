
#ifndef PVAACFFREC_PLUGIN_H_INCLUDED
#define PVAACFFREC_PLUGIN_H_INCLUDED

#ifndef PVMF_RECOGNIZER_PLUGIN_H_INCLUDED
#include "pvmf_recognizer_plugin.h"
#endif

class PVAACFFRecognizerPlugin : public PVMFRecognizerPluginInterface
{
    public:
        PVAACFFRecognizerPlugin()
        {
        };

        ~PVAACFFRecognizerPlugin()
        {
        };

        PVMFStatus SupportedFormats(PVMFRecognizerMIMEStringList& aSupportedFormatsList);

        PVMFStatus Recognize(PVMFDataStreamFactory& aSourceDataStreamFactory, PVMFRecognizerMIMEStringList* aFormatHint,
                             Oscl_Vector<PVMFRecognizerResult, OsclMemAllocator>& aRecognizerResult);

        PVMFStatus GetRequiredMinBytesForRecognition(uint32& aBytes);
};

#endif // PVAACFFREC_PLUGIN_H_INCLUDED

