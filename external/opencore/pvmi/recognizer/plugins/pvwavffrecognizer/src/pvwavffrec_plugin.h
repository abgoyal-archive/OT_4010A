
#ifndef PVWAVFFREC_PLUGIN_H_INCLUDED
#define PVWAVFFREC_PLUGIN_H_INCLUDED

#ifndef PVMF_RECOGNIZER_PLUGIN_H_INCLUDED
#include "pvmf_recognizer_plugin.h"
#endif

#define WAVFF_MIN_DATA_SIZE_FOR_RECOGNITION 12

class PVWAVFFRecognizerPlugin : public PVMFRecognizerPluginInterface
{
    public:
        PVWAVFFRecognizerPlugin()
        {
        };

        ~PVWAVFFRecognizerPlugin()
        {
        };

        PVMFStatus SupportedFormats(PVMFRecognizerMIMEStringList& aSupportedFormatsList);

        PVMFStatus Recognize(PVMFDataStreamFactory& aSourceDataStreamFactory,
                             PVMFRecognizerMIMEStringList* aFormatHint,
                             Oscl_Vector<PVMFRecognizerResult, OsclMemAllocator>& aRecognizerResult);

        PVMFStatus GetRequiredMinBytesForRecognition(uint32& aBytes);
};

#endif // PVWAVFFREC_PLUGIN_H_INCLUDED

