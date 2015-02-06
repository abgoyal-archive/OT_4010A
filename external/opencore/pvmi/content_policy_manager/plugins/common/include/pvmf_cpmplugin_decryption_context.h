
#ifndef PVMF_CPMPLUGIN_DECRYPTION_CONTEXT_H_INCLUDED
#define PVMF_CPMPLUGIN_DECRYPTION_CONTEXT_H_INCLUDED

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

#define PVMFCPMPluginWMDRMDecryptContextUuid PVUuid(0x19c4fdb8,0x2165,0x4f10,0xa5,0x53,0x72,0x76,0xd7,0xc6,0xb1,0x2a)

class PVMFCPMPluginWMDRMDecryptContext : public PVInterface
{
    public:
        PVMFCPMPluginWMDRMDecryptContext()
        {
            iRef = 0;
            Oscl_Int64_Utils::set_uint64(iSampleID, 0, 0);
            iMediaObjectStartOffset = 0;
        };

        virtual ~PVMFCPMPluginWMDRMDecryptContext()
        {
        };

        void addRef()
        {
            iRef++;
        };

        void removeRef()
        {
            iRef--;
        };

        bool queryInterface(const PVUuid& uuid,
                            PVInterface*& iface)
        {
            iface = NULL;
            if (uuid == PVMFCPMPluginWMDRMDecryptContextUuid)
            {
                iface = OSCL_STATIC_CAST(PVInterface*, this);
                return true;
            }
            return false;
        };

        uint64 iSampleID;
        uint32 iMediaObjectStartOffset;

    private:
        uint32 iRef;
};

#endif //PVMF_CPMPLUGIN_DECRYPTION_CONTEXT_H_INCLUDED

