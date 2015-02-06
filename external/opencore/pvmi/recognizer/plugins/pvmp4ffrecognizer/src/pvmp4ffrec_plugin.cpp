
#include "pvmp4ffrec_plugin.h"
#include "pvmp4ffrec.h"
#include "oscl_file_io.h"


PVMFStatus PVMP4FFRecognizerPlugin::SupportedFormats(PVMFRecognizerMIMEStringList& aSupportedFormatsList)
{
    // Return MP4 as supported type
    OSCL_HeapString<OsclMemAllocator> supportedformat = PVMF_MIME_MPEG4FF;
    aSupportedFormatsList.push_back(supportedformat);
    return PVMFSuccess;
}


PVMFStatus PVMP4FFRecognizerPlugin::Recognize(PVMFDataStreamFactory& aSourceDataStreamFactory, PVMFRecognizerMIMEStringList* aFormatHint,
        Oscl_Vector<PVMFRecognizerResult, OsclMemAllocator>& aRecognizerResult)
{
    OSCL_UNUSED_ARG(aFormatHint);

    // Check if the specified file is MP4 file by using MP4 parser lib's static function
    bool ismp4file = false;
    OSCL_wHeapString<OsclMemAllocator> tmpfilename;
    int32 leavecode = 0;
    OSCL_TRY(leavecode, ismp4file = MP4FileRecognizer::IsMP4File(&aSourceDataStreamFactory));
    OSCL_FIRST_CATCH_ANY(leavecode,
                         return PVMFErrNoMemory;
                        );

    if (ismp4file == true)
    {
        // It is an MP4 file so add positive result
        PVMFRecognizerResult result;
        result.iRecognizedFormat = PVMF_MIME_MPEG4FF;
        result.iRecognitionConfidence = PVMFRecognizerConfidenceCertain;
        aRecognizerResult.push_back(result);
    }

    return PVMFSuccess;
}

PVMFStatus PVMP4FFRecognizerPlugin::GetRequiredMinBytesForRecognition(uint32& aBytes)
{
    aBytes = 128;
    return PVMFSuccess;
}






