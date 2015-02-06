
#ifndef PVMP4FFREC_PLUGIN_H_INCLUDED
#define PVMP4FFREC_PLUGIN_H_INCLUDED

#ifndef PVMF_RECOGNIZER_PLUGIN_H_INCLUDED
#include "pvmf_recognizer_plugin.h"
#endif

class PVMP4FFRecognizerPlugin : public PVMFRecognizerPluginInterface
{
    public:
        PVMP4FFRecognizerPlugin()
        {
        };

        ~PVMP4FFRecognizerPlugin()
        {
        };

        PVMFStatus SupportedFormats(PVMFRecognizerMIMEStringList& aSupportedFormatsList);

        PVMFStatus Recognize(PVMFDataStreamFactory& aSourceDataStreamFactory, PVMFRecognizerMIMEStringList* aFormatHint,
                             Oscl_Vector<PVMFRecognizerResult, OsclMemAllocator>& aRecognizerResult);

        PVMFStatus GetRequiredMinBytesForRecognition(uint32& aBytes);
};

#endif // PVMP4FFREC_PLUGIN_H_INCLUDED

