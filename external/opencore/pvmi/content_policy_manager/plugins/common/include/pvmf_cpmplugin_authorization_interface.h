
#ifndef PVMF_CPMPLUGIN_AUTHORIZATION_INTERFACE_H_INCLUDED
#define PVMF_CPMPLUGIN_AUTHORIZATION_INTERFACE_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef OSCL_TYPES_H_INCLUDED
#include "oscl_types.h"
#endif
#ifndef PV_UUID_H_INCLUDED
#include "pv_uuid.h"
#endif
#ifndef PV_INTERFACE_H_INCLUDED
#include "pv_interface.h"
#endif
#ifndef PVMI_KVP_INCLUDED
#include "pvmi_kvp.h"
#endif

#define PVMF_CPMPLUGIN_AUTHORIZATION_INTERFACE_MIMETYPE "pvxxx/pvmf/cpm/plugin/authorization_interface"
#define PVMFCPMPluginAuthorizationInterfaceUuid PVUuid(0xfd47907a,0xffde,0x463e,0xa3,0x35,0xd4,0x98,0xff,0xdf,0xd0,0xea)

class PVMFCPMPluginAuthorizationInterface : public PVInterface
{
    public:
        /**
         * Asynchronous method to authorize usage with a plug-in.
         *
         * @param aSessionId The assigned plugin session ID to use for this request
         * @param aRequestedUsage The kvp that contains the requested usage.
         * @param aApprovedUsage The kvp that contains the approved intent.
         *
         * @returns A unique command id for asynchronous completion.
         */
        virtual PVMFCommandId AuthorizeUsage(PVMFSessionId aSessionId,
                                             PvmiKvp& aRequestedUsage,
                                             PvmiKvp& aApprovedUsage,
                                             PvmiKvp& aAuthorizationData,
                                             uint32&  aRequestTimeOutInMS,
                                             OsclAny* aContext = NULL) = 0;

        virtual PVMFCommandId UsageComplete(PVMFSessionId aSessionId,
                                            const OsclAny* aContext = NULL) = 0;

};


#endif //PVMF_CPMPLUGIN_AUTHORIZATION_INTERFACE_H_INCLUDED

