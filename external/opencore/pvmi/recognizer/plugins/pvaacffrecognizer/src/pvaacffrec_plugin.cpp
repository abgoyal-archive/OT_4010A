
#include "pvaacffrec_plugin.h"
#include "aacfileparser.h"
#include "oscl_file_io.h"

#define PVMF_AAC_REC_PLUGIN_MIN_SIZE_FOR_RECOGNITION 512

PVMFStatus PVAACFFRecognizerPlugin::SupportedFormats(PVMFRecognizerMIMEStringList& aSupportedFormatsList)
{
    // Return AAC as supported type
    OSCL_HeapString<OsclMemAllocator> supportedformat = PVMF_MIME_AACFF;
    aSupportedFormatsList.push_back(supportedformat);
    return PVMFSuccess;
}


PVMFStatus
PVAACFFRecognizerPlugin::Recognize(PVMFDataStreamFactory& aSourceDataStreamFactory,
                                   PVMFRecognizerMIMEStringList* aFormatHint,
                                   Oscl_Vector<PVMFRecognizerResult, OsclMemAllocator>& aRecognizerResult)
{
    OSCL_UNUSED_ARG(aFormatHint);
    OSCL_wStackString<1> tmpfilename;
    int32 leavecode = 0;
    CAACFileParser* aacfile = NULL;
    OSCL_TRY(leavecode, aacfile = OSCL_NEW(CAACFileParser, ()));
    if (leavecode || aacfile == NULL)
    {
        return PVMFErrNoMemory;
    }

    ParserErrorCode retVal = aacfile->IsAACFile(tmpfilename, NULL, &aSourceDataStreamFactory);

    OSCL_DELETE(aacfile);
    aacfile = NULL;

    if (OK == retVal)
    {
        // It is an aac file so add positive result
        PVMFRecognizerResult result;
        result.iRecognizedFormat = PVMF_MIME_AACFF;
        result.iRecognitionConfidence = PVMFRecognizerConfidenceCertain;
        aRecognizerResult.push_back(result);
    }
    else if (AAC_INSUFFICIENT_DATA == retVal) // sam sun for aac rough duration
    {
        // It may be an aac file, but there is not enough data to make the determination
        PVMFRecognizerResult result;
        result.iRecognizedFormat = PVMF_MIME_AACFF;
        result.iRecognitionConfidence = PVMFRecognizerConfidencePossible;
        aRecognizerResult.push_back(result);
    }

    return PVMFSuccess;
}

PVMFStatus PVAACFFRecognizerPlugin::GetRequiredMinBytesForRecognition(uint32& aBytes)
{
    aBytes = PVMF_AAC_REC_PLUGIN_MIN_SIZE_FOR_RECOGNITION;
    return PVMFSuccess;
}






