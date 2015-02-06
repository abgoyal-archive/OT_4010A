
#include "pvrmffrec_plugin.h"
#include "pvfile.h"
#include "oscl_file_io.h"
#include <cutils/log.h>

#define RM_HEADER_OBJECT  0x464D522E   /* '.RMF' */

PVMFStatus PVRMFFRecognizerPlugin::SupportedFormats(PVMFRecognizerMIMEStringList& aSupportedFormatsList)
{
    OSCL_HeapString<OsclMemAllocator> supportedformat = PVMF_MIME_RMFF;
    aSupportedFormatsList.push_back(supportedformat);
    return PVMFSuccess;
}


PVMFStatus PVRMFFRecognizerPlugin::Recognize(PVMFDataStreamFactory& aSourceDataStreamFactory, PVMFRecognizerMIMEStringList* aFormatHint,
        Oscl_Vector<PVMFRecognizerResult, OsclMemAllocator>& aRecognizerResult)
{
    //LOGE ("[Morris] +PVRMFFRecognizerPlugin::Recognize()\n");
    OSCL_UNUSED_ARG(aFormatHint);
    OSCL_wHeapString<OsclMemAllocator> tmpfilename;
    Oscl_FileServer fileServ;
    PVFile pvfile;
    pvfile.SetCPM(&aSourceDataStreamFactory);

    if (!(pvfile.Open(tmpfilename.get_cstr(), Oscl_File::MODE_READ | Oscl_File::MODE_BINARY, fileServ)))
    {
        uint32* readData = NULL;

        readData = (uint32*)(oscl_malloc(sizeof(uint32)));
        if (readData != NULL)
        {
            int bytesRead = 0;
            bytesRead = pvfile.Read(readData, 1, 4);
            //LOGE ("[Morris] bytesRead = %d, data=0x%x\n", bytesRead, (*readData));
            if (bytesRead != sizeof(uint32))
            {
            	LOGE ("[Morris] -PVRMFFRecognizerPlugin::Recognize() return PVMFFailure 1\n");
                pvfile.Close();
                oscl_free(readData);
                return PVMFFailure;
            }

            if ((*readData) == RM_HEADER_OBJECT)
            {
                    PVMFRecognizerResult result;
                    result.iRecognizedFormat = PVMF_MIME_RMFF;
                    LOGE ("[Morris] PVRMFFRecognizerPlugin::Recognize recognized RM format!!!!\n");
                    result.iRecognitionConfidence = PVMFRecognizerConfidenceCertain;
                    aRecognizerResult.push_back(result);
            }

            pvfile.Close();
            oscl_free(readData);
            //LOGE ("[Morris] PVRMFFRecognizerPlugin::Recognize return PVMFSuccess!!!\n");
            return PVMFSuccess;
        }
        else
        {
            pvfile.Close();
            oscl_free(readData);
            LOGE ("[Morris] -PVRMFFRecognizerPlugin::Recognize() return PVMFFailure 2\n");
            return PVMFFailure;
        }
    }
    else
    {
    	LOGE ("[Morris] -PVRMFFRecognizerPlugin::Recognize() return PVMFFailure 3\n");
        return PVMFFailure;
    }

    return PVMFSuccess;
}

PVMFStatus PVRMFFRecognizerPlugin::GetRequiredMinBytesForRecognition(uint32& aBytes)
{
    aBytes = 16;
    return PVMFSuccess;
}






