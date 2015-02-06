
#include "pvlogger.h"

#include "pv_omxcore.h"
#include "omx_interface.h"



class PVOMXInterface : public OMXInterface
{
    public:
        OsclAny* SharedLibraryLookup(const OsclUuid& aInterfaceId)
        {
            if (aInterfaceId == OMX_INTERFACE_ID)
            {
                // the library lookup was successful
                return this;
            }
            // the ID doesn't match
            return NULL;
        };

        static PVOMXInterface* Instance()
        {
            return OSCL_NEW(PVOMXInterface, ());
        };

        bool UnloadWhenNotUsed(void)
        {
            // As of 9/22/08, the PV OMX core library can not be
            // safely unloaded and reloaded when the proxy interface
            // is enabled.
            return false;
        };

    private:

        PVOMXInterface()
        {
            // set the pointers to the omx core methods
            pOMX_Init = OMX_Init;
            pOMX_Deinit = OMX_Deinit;
            pOMX_ComponentNameEnum = OMX_ComponentNameEnum;
            pOMX_GetHandle = OMX_GetHandle;
            pOMX_FreeHandle = OMX_FreeHandle;
            pOMX_GetComponentsOfRole = OMX_GetComponentsOfRole;
            pOMX_GetRolesOfComponent = OMX_GetRolesOfComponent;
            pOMX_SetupTunnel = OMX_SetupTunnel;
            pOMX_GetContentPipe = OMX_GetContentPipe;
            pOMXConfigParser = OMXConfigParser;
        };

};

// function to obtain the interface object from the shared library
extern "C"
{
    OSCL_EXPORT_REF OsclAny* PVGetInterface()
    {
        return PVOMXInterface::Instance();
    }
    OSCL_EXPORT_REF void PVReleaseInterface(void* interface)
    {
        PVOMXInterface* pInterface = (PVOMXInterface*)interface;
        if (pInterface)
        {
            OSCL_DELETE(pInterface);
        }
    }
}
