
#ifndef PVMP3FFREC_PLUGIN_H_INCLUDED
#define PVMP3FFREC_PLUGIN_H_INCLUDED

#ifndef PVMF_RECOGNIZER_PLUGIN_H_INCLUDED
#include "pvmf_recognizer_plugin.h"
#endif

class PVMP3FFRecognizerPlugin : public PVMFRecognizerPluginInterface
{
    public:
        PVMP3FFRecognizerPlugin()
        {
        };

        ~PVMP3FFRecognizerPlugin()
        {
        };

        PVMFStatus SupportedFormats(PVMFRecognizerMIMEStringList& aSupportedFormatsList);

        PVMFStatus Recognize(PVMFDataStreamFactory& aSourceDataStreamFactory, PVMFRecognizerMIMEStringList* aFormatHint,
                             Oscl_Vector<PVMFRecognizerResult, OsclMemAllocator>& aRecognizerResult);

        PVMFStatus GetRequiredMinBytesForRecognition(uint32& aBytes);
};

#endif // PVMP3FFREC_PLUGIN_H_INCLUDED

