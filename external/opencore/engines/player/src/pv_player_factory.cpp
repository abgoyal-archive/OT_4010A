
#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef PV_PLAYER_INTERFACE_H_INCLUDED
#include "pv_player_interface.h"
#endif

#ifndef PV_ENGINE_OBSERVER_H_INCLUDED
#include "pv_engine_observer.h"
#endif

#ifndef PV_PLAYER_ENGINE_H_INCLUDED
#include "pv_player_engine.h"
#endif

#ifndef PV_PLAYER_FACTORY_H_INCLUDED
#include "pv_player_factory.h"
#endif

#ifndef OSCL_DLL_H_INCLUDED
#include "oscl_dll.h"
#endif

// Define entry point for this DLL
OSCL_DLL_ENTRY_POINT_DEFAULT()

OSCL_EXPORT_REF PVPlayerInterface *PVPlayerFactory::CreatePlayer(PVCommandStatusObserver* aCmdStatusObserver,
        PVErrorEventObserver *aErrorEventObserver,
        PVInformationalEventObserver *aInfoEventObserver,
        uint32* pUserData,
        bool aHwAccelerated)
{
    return PVPlayerEngine::New(aCmdStatusObserver, aErrorEventObserver, aInfoEventObserver, aHwAccelerated, pUserData);
}

OSCL_EXPORT_REF bool PVPlayerFactory::DeletePlayer(PVPlayerInterface* aPlayer)
{
    PVPlayerEngine* engptr = (PVPlayerEngine*)aPlayer;
    OSCL_DELETE(engptr);

    return true;
}


