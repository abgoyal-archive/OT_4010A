
#include "pvmp3ffrec_plugin.h"
#include "imp3ff.h"
#include "oscl_file_io.h"
#include <cutils/log.h>

#define PVMF_MP3_REC_PLUGIN_MIN_SIZE_FOR_RECOGNITION 128
//intial scan length for recognition, for now 200kb
// <--- Morris Yang ALPS00122654 reduce scan size
//#define PVMF_MP3_REC_PLUGIN_FILE_SCAN_SIZE (200*1024)
#define PVMF_MP3_REC_PLUGIN_FILE_SCAN_SIZE (8*1024)
// --->

//#define ENABLE_LOG_pvmp3ffrec_plugin

#ifdef ENABLE_LOG_pvmp3ffrec_plugin
#undef LOGV
#define LOGV(...) LOGD(__VA_ARGS__)
#endif

PVMFStatus PVMP3FFRecognizerPlugin::SupportedFormats(PVMFRecognizerMIMEStringList& aSupportedFormatsList)
{
    // Return MP3 as supported type
    OSCL_HeapString<OsclMemAllocator> supportedformat = PVMF_MIME_MP3FF;
    aSupportedFormatsList.push_back(supportedformat);
    return PVMFSuccess;
}


PVMFStatus PVMP3FFRecognizerPlugin::Recognize(PVMFDataStreamFactory& aSourceDataStreamFactory, PVMFRecognizerMIMEStringList* aFormatHint,
        Oscl_Vector<PVMFRecognizerResult, OsclMemAllocator>& aRecognizerResult)
{
    OSCL_UNUSED_ARG(aFormatHint);
    LOGV("PVMP3FFRecognizerPlugin::Recognize");
    // Instantiate the IMpeg3File object, which is the class representing the mp3 ff parser library.
    OSCL_wStackString<1> tmpfilename;
    MP3ErrorType eSuccess = MP3_SUCCESS;
    uint32 initSearchSize = PVMF_MP3_REC_PLUGIN_FILE_SCAN_SIZE;
    IMpeg3File* mp3File = OSCL_NEW(IMpeg3File, (eSuccess));
    if (!mp3File || eSuccess != MP3_SUCCESS)
    {
        // unable to construct parser object
        return PVMFSuccess;
    }

    eSuccess = mp3File->IsMp3File(tmpfilename, &aSourceDataStreamFactory, initSearchSize);
    LOGV("mp3File->IsMp3File result=%d", eSuccess);

    PVMFRecognizerResult result;
    if (eSuccess == MP3_SUCCESS)
    {
        LOGV("It is an MP3 file so add positive result");
        // It is an MP3 file so add positive result
        result.iRecognizedFormat = PVMF_MIME_MP3FF;
        result.iRecognitionConfidence = PVMFRecognizerConfidenceCertain;
        aRecognizerResult.push_back(result);
    }
    else if (eSuccess == MP3_INSUFFICIENT_DATA)
    {
        LOGV("It could be an MP3 file, but not sure");
        // It could be an MP3 file, but not sure
        result.iRecognizedFormat = PVMF_MIME_MP3FF;
        result.iRecognitionConfidence = PVMFRecognizerConfidencePossible;
        aRecognizerResult.push_back(result);
    }
    if (mp3File)
    {
        OSCL_DELETE(mp3File);
        mp3File = NULL;
    }
    return PVMFSuccess;
}

PVMFStatus PVMP3FFRecognizerPlugin::GetRequiredMinBytesForRecognition(uint32& aBytes)
{
    aBytes = PVMF_MP3_REC_PLUGIN_MIN_SIZE_FOR_RECOGNITION;
    return PVMFSuccess;
}






