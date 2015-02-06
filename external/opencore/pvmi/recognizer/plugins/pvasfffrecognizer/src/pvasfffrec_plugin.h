
#ifndef PVASFFFREC_PLUGIN_H_INCLUDED
#define PVASFFFREC_PLUGIN_H_INCLUDED

#ifndef PVMF_RECOGNIZER_PLUGIN_H_INCLUDED
#include "pvmf_recognizer_plugin.h"
#endif

class PVASFFFRecognizerPlugin : public PVMFRecognizerPluginInterface
{
    public:
        PVASFFFRecognizerPlugin()
        {
        };

        ~PVASFFFRecognizerPlugin()
        {
        };

        PVMFStatus SupportedFormats(PVMFRecognizerMIMEStringList& aSupportedFormatsList);

        PVMFStatus Recognize(PVMFDataStreamFactory& aSourceDataStreamFactory, PVMFRecognizerMIMEStringList* aFormatHint,
                             Oscl_Vector<PVMFRecognizerResult, OsclMemAllocator>& aRecognizerResult);

        PVMFStatus GetRequiredMinBytesForRecognition(uint32& aBytes);
};

#endif // PVASFFFREC_PLUGIN_H_INCLUDED

