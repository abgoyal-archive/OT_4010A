
#ifndef PVMF_RECOGNIZER_TYPES_H_INCLUDED
#define PVMF_RECOGNIZER_TYPES_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif

#ifndef OSCL_STRING_H_INCLUDED
#include "oscl_string.h"
#endif

#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif

#ifndef PVMF_EVENT_HANDLING_H_INCLUDED
#include "pvmf_event_handling.h"
#endif


typedef Oscl_Vector<OSCL_HeapString<OsclMemAllocator>, OsclMemAllocator> PVMFRecognizerMIMEStringList;

typedef enum _PVMFRecognizerConfidence
{
    PVMFRecognizerConfidenceNotCertain,     // 100% sure not the format
    PVMFRecognizerConfidenceNotPossible,    // Maybe not the format
    PVMFRecognizerConfidenceUnknown,        // Not sure one way or the other
    PVMFRecognizerConfidencePossible,       // Maybe the format
    PVMFRecognizerConfidenceCertain         // 100% sure of the format
} PVMFRecognizerConfidence;


class PVMFRecognizerResult
{
    public:
        PVMFRecognizerResult()
        {
        };

        // Copy constructor for use in Oscl_Vector
        PVMFRecognizerResult(const PVMFRecognizerResult& aSrc)
        {
            iRecognizedFormat = aSrc.iRecognizedFormat;
            iRecognitionConfidence = aSrc.iRecognitionConfidence;
            //  iRecognizerSubFormatList=aSrc.iRecognizerSubFormatList;
        };

        ~PVMFRecognizerResult()
        {
        };

        // The format of interest as a MIME string
        OSCL_HeapString<OsclMemAllocator> iRecognizedFormat;
        // The confidence level of recognition
        PVMFRecognizerConfidence iRecognitionConfidence;
        // If the format is a container format, the format of content within
//  Oscl_Vector<PVMFRecognizerResult, OsclMemAllocator> iRecognizerSubFormatList;
};


class PVMFRecognizerCommmandHandler
{
    public:
        /**
         * Handle an event that has been generated.
         *
         * @param aResponse
         *        The response to a previously issued command
         */
        virtual void RecognizerCommandCompleted(const PVMFCmdResp& aResponse) = 0;
        virtual ~PVMFRecognizerCommmandHandler() {}
};

#endif // PVMF_RECOGNIZER_TYPES_H_INCLUDED

