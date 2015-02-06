

#ifndef ANDROID_AUDIO_STREAM_H
#define ANDROID_AUDIO_STREAM_H

#include <utils/Errors.h>

#include "android_audio_mio.h"
#include "utils/Timers.h"

using namespace android;

class AndroidAudioStream : public AndroidAudioMIO
{
public:
    OSCL_IMPORT_REF AndroidAudioStream();
    OSCL_IMPORT_REF ~AndroidAudioStream();

    virtual PVMFCommandId QueryUUID(const PvmfMimeString& aMimeType, Oscl_Vector<PVUuid,
            OsclMemAllocator>& aUuids, bool aExactUuidsOnly=false,
            const OsclAny* aContext=NULL);

    virtual PVMFCommandId QueryInterface(const PVUuid& aUuid, PVInterface*& aInterfacePtr,
            const OsclAny* aContext=NULL);

    virtual void setParametersSync(PvmiMIOSession aSession, PvmiKvp* aParameters,
            int num_elements, PvmiKvp * & aRet_kvp);

private:

    virtual void writeAudioBuffer(uint8* aData, uint32 aDataLen, PVMFCommandId cmdId,
            OsclAny* aContext, PVMFTimestamp aTimestamp);

    // active timing
    AndroidAudioMIOActiveTimingSupport* iActiveTiming;

    // number of bytes in an input frame
    int iInputFrameSizeInBytes;
    bool mClockUpdated;
};

#endif // ANDROID_AUDIO_STREAM_H

