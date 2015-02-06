
#ifndef PVMF_CPMPLUGIN_FACTORY_REGISTRY_H_INCLUDED
#define PVMF_CPMPLUGIN_FACTORY_REGISTRY_H_INCLUDED

#ifndef OSCL_STRING_H_INCLUDED
#include "oscl_string.h"
#endif
#ifndef PVMF_RETURN_CODES_H_INCLUDED
#include "pvmf_return_codes.h"
#endif

class PVMFCPMPluginFactory;
class OsclRegistryClient;

#define PVMF_MIME_CPM_PLUGIN    "X-CPM-PLUGIN"

class PVMFCPMPluginFactoryRegistryClient
{
    public:
        OSCL_IMPORT_REF PVMFCPMPluginFactoryRegistryClient();
        OSCL_IMPORT_REF ~PVMFCPMPluginFactoryRegistryClient();

        /*!
         * Connect
         *
         *    The application code calls this to connect to the registry
         *    server.
         *    @param: flag to select per-thread registry as opposed to global
         *       registry.
         *    @return: PVMFSuccess, or one of the PVMF return codes for errors.
         */
        OSCL_IMPORT_REF PVMFStatus Connect(bool aPerThreadRegistry = false);

        /*!
         * RegisterPlugin
         *
         *    The application code calls this to register a plugin factory
         *    function.
         *
         *    @param: Plugin mime type, of the form X-CPM-PLUGIN/...
         *    @param: Plugin factory implementation.
         *    @return: PVMFSuccess, or one of the PVMF return codes for errors.
         */
        OSCL_IMPORT_REF PVMFStatus RegisterPlugin(OSCL_String& aMimeType,
                PVMFCPMPluginFactory& aFactory) ;
        /*!
         * UnRegisterPlugin
         *
         *    The application code can call this to un-register a CPM
         *    plugin factory.
         *
         *    Note: This call is optional.  The plugin registry will
         *      be automatically cleaned up when Close is called.
         *
         *    @param: Plugin mime type
         *    @return: PVMFSuccess, or one of the PVMF return codes for errors.
         */
        OSCL_IMPORT_REF PVMFStatus UnRegisterPlugin(OSCL_String& aMimeType) ;

        /*!
         * Close
         *
         *    The application code calls this to close the registry client
         *    session and cleanup all registered plugins.
         *
         */
        OSCL_IMPORT_REF void Close();

    private:
        OsclRegistryClient* iClient;

};



#endif //PVMF_CPMPLUGIN_INTERFACE_H_INCLUDED

