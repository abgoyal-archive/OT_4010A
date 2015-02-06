

#ifndef PVMF_FILEINPUT_NODE_EXTENSION_H_INCLUDED
#define PVMF_FILEINPUT_NODE_EXTENSION_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef PV_INTERFACE_H
#include "pv_interface.h"
#endif
#ifndef PV_UUID_H_INCLUDED
#include "pv_uuid.h"
#endif

// The PVUuid cannot by a const or it will create uninitialized data
//
#define KPVMFFileInputNodeExtensionUuid PVUuid(0xca27cb64,0x83ed,0x40d6,0x96,0xa3,0xed,0x1d,0x8b,0x60,0x11,0x38)

// Forward declaration
class OsclRefCounterMemFrag;

class PVMFFileInputNodeExtensionInterface : public PVInterface
{
    public:
        virtual void addRef() = 0;
        virtual void removeRef() = 0;
        virtual bool queryInterface(const PVUuid& uuid, PVInterface*& iface) = 0;

        /**
         * Retrieves bitstream configuration information specific to the file type.
         * This method is not valid for all files types. For M4V files, this function
         * would copy the VOL header to the config memory fragment provided. For
         * other file types, this method would return false, and aConfig is not
         * modified.
         *
         * @param aConfig Memory fragment to hold the config information.
         * @return true if config info is provided, else false.
         */
        virtual bool GetBitstreamConfig(OsclRefCounterMemFrag& aConfig) = 0;
};

#endif // PVMF_FILEINPUT_NODE_EXTENSION_H_INCLUDED
