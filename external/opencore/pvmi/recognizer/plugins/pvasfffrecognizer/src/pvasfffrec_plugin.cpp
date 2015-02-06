
#include "pvasfffrec_plugin.h"
//#include "pvasfffrec.h"
#include "oscl_file_io.h"


PVMFStatus PVASFFFRecognizerPlugin::SupportedFormats(PVMFRecognizerMIMEStringList& aSupportedFormatsList)
{
    // Return ASF as supported type
    OSCL_HeapString<OsclMemAllocator> supportedformat = PVMF_MIME_ASFFF;
    aSupportedFormatsList.push_back(supportedformat);
    return PVMFSuccess;
}


PVMFStatus PVASFFFRecognizerPlugin::Recognize(PVMFDataStreamFactory& aSourceDataStreamFactory, PVMFRecognizerMIMEStringList* aFormatHint,
        Oscl_Vector<PVMFRecognizerResult, OsclMemAllocator>& aRecognizerResult)
{
    return PVMFSuccess;
}

PVMFStatus PVASFFFRecognizerPlugin::GetRequiredMinBytesForRecognition(uint32& aBytes)
{
    return PVMFSuccess;
}






