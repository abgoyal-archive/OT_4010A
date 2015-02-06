

#ifndef PVMF_FILEOUTPUT_CONFIG_H_INCLUDED
#define PVMF_FILEOUTPUT_CONFIG_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef OSCL_STRING_H_INCLUDED
#include "oscl_string.h"
#endif
#ifndef PV_UUID_H_INCLUDED
#include "pv_uuid.h"
#endif
#ifndef PV_INTERFACE_H_INCLUDED
#include "pv_interface.h"
#endif
#ifndef PVMF_FORMAT_TYPE_H_INCLUDED
#include "pvmf_format_type.h"
#endif
#define PVMF_MIME_FILE_OUTPUT _STRLIT_CHAR("x-pvmf/file/output")
#define PVMF_MIME_MIN_COMP_CNT 4
#define PVMF_MIME_MIN_COMP_IDX 3
#define PVMFDummyFileOutputNodeConfigUuid PVUuid(0x5411cc90,0xe335,0x4b06,0xbe,0xe4,0x7c,0x81,0x40,0x6a,0xf3,0xaa)

class PVMFDummyFileOutputNodeConfigInterface : public PVInterface
{
    public:
        /**
         * Register a reference to this interface.
         */
        virtual void addRef() = 0;

        /**
         * Remove a reference to this interface.
         */
        virtual void removeRef() = 0;

        /**
         * Query for an instance of a particular interface.
         *
         * @param uuid Uuid of the requested interface
         * @param iface Output parameter where pointer to an instance of the
         * requested interface will be stored if it is supported by this object
         * @return true if the requested interface is supported, else false
         */
        virtual bool queryInterface(const PVUuid& uuid, PVInterface*& iface) = 0;

        /**
         * This method sets the output file name. This method must be called before
         * the engine / node is initialized.
         *
         * @param aFileName Output file name
         * @return Completion status of this method.
         */
        virtual PVMFStatus SetOutputFileName(const OSCL_wString& aFileName) = 0;

};

#endif // PVMF_FILEOUTPUT_CONFIG_H_INCLUDED


