
#ifndef PVMI_MIO_COMM_LOOPBACK_FACTORY_H_INCLUDED
#define PVMI_MIO_COMM_LOOPBACK_FACTORY_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif
#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif
#ifndef PVMF_FORMAT_TYPE_H_INCLUDED
#include "pvmf_format_type.h"
#endif
#ifndef PVMF_TIMESTAMP_H_INCLUDED
#include "pvmf_timestamp.h"
#endif

// Forward declarations
class PvmiMIOControl;
class OsclMemAllocator;
class PvmiMIOCommLoopbackTestObserver;

class PvmiMIOCommLoopbackSettings
{
    public:
        PvmiMIOCommLoopbackSettings()
        {
            iMediaFormat = PVMF_MIME_FORMAT_UNKNOWN;
            iTestObserver = NULL;
        }

        PvmiMIOCommLoopbackSettings(const PvmiMIOCommLoopbackSettings& aSettings)
        {
            iMediaFormat = aSettings.iMediaFormat;
            iTestObserver = aSettings.iTestObserver;
        }
        ~PvmiMIOCommLoopbackSettings() {}

        // General settings
        PVMFFormatType iMediaFormat;
        PvmiMIOCommLoopbackTestObserver *iTestObserver;
};


class PvmiMIOCommLoopbackFactory
{
    public:
        /**
         * Creates an instance of PvmiMIOCommLoopback. If the creation fails,
         * this function will leave.
         *
         * @param aSettings Settings for PvmiMIOCommLoopback
         * @returns A pointer to an PvmiMIOControl for the file input media input module
         * @throw Leaves with OsclErrNoMemory if memory allocation failed.
         */
        OSCL_IMPORT_REF static PvmiMIOControl* Create(const PvmiMIOCommLoopbackSettings& aSettings);

        /**
         * This function allows the application to delete an instance of file input MIO module
         * and reclaim all allocated resources.  An instance can be deleted only in
         * the idle state. An attempt to delete in any other state will fail and return false.
         *
         * @param aNode The file input MIO module to be deleted.
         * @returns A status code indicating success or failure.
         */
        OSCL_IMPORT_REF static bool Delete(PvmiMIOControl* aMio);
};

//An observer class for test support.
class PvmiMIOCommLoopbackTestObserver
{
    public:
        OSCL_IMPORT_REF virtual void LoopbackPos(PVMFTimestamp& aTimestamp) = 0;
};

#endif // PVMIO_COMM_LOOPBACK_FACTORY_H_INCLUDED
