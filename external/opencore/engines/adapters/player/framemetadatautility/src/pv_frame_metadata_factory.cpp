
#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef PV_FRAME_METADATA_INTERFACE_H_INCLUDED
#include "pv_frame_metadata_interface.h"
#endif

#ifndef PV_ENGINE_OBSERVER_H_INCLUDED
#include "pv_engine_observer.h"
#endif

#ifndef PV_FRAME_METADATA_UTILITY_H_INCLUDED
#include "pv_frame_metadata_utility.h"
#endif

#ifndef PV_FRAME_METADATA_FACTORY_H_INCLUDED
#include "pv_frame_metadata_factory.h"
#endif

#ifndef OSCL_DLL_H_INCLUDED
#include "oscl_dll.h"
#endif

// Define entry point for this DLL
OSCL_DLL_ENTRY_POINT_DEFAULT()



OSCL_EXPORT_REF PVFrameAndMetadataInterface *PVFrameAndMetadataFactory::CreateFrameAndMetadataUtility(char *aOutputFormatMIMEType,
        PVCommandStatusObserver* aCmdStatusObserver,
        PVErrorEventObserver *aErrorEventObserver,
        PVInformationalEventObserver *aInfoEventObserver,
        bool aHwAccelerated)
{
    return PVFrameAndMetadataUtility::New(aOutputFormatMIMEType, aCmdStatusObserver, aErrorEventObserver, aInfoEventObserver, aHwAccelerated);
}


OSCL_EXPORT_REF bool PVFrameAndMetadataFactory::DeleteFrameAndMetadataUtility(PVFrameAndMetadataInterface* aUtility)
{
    PVFrameAndMetadataUtility* utilptr = (PVFrameAndMetadataUtility*)aUtility;
    OSCL_DELETE(utilptr);

    return true;
}


