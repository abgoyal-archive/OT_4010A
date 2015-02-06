
#ifndef PVMI_KVP_UTIL_H_INCLUDED
#define PVMI_KVP_UTIL_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef PVMI_KVP_H_INCLUDED
#include "pvmi_kvp.h"
#endif

#ifndef PVMF_RETURN_CODES_H_INCLUDED
#include "pvmf_return_codes.h"
#endif

OSCL_IMPORT_REF PvmiKvpType GetTypeFromKeyString(PvmiKeyType aKeyString);

OSCL_IMPORT_REF PvmiKvpAttr GetAttrTypeFromKeyString(PvmiKeyType aKeyString);

OSCL_IMPORT_REF PvmiKvpValueType GetValTypeFromKeyString(PvmiKeyType aKeyString);


class PVMFCreateKVPUtils
{
    public:
        OSCL_IMPORT_REF static PVMFStatus CreateKVPForWStringValue(PvmiKvp& aKeyVal,
                const char* aKeyTypeString,
                OSCL_wString& aValString,
                char* aMiscKeyParam = NULL,
                uint32 aMaxSize = 0xFFFFFFFF,
                uint32 aTruncateFlag = 1);

        OSCL_IMPORT_REF static PVMFStatus CreateKVPForCharStringValue(PvmiKvp& aKeyVal,
                const char* aKeyTypeString,
                const char* aValString,
                char* aMiscKeyParam = NULL,
                uint32 aMaxSize = 0xFFFFFFFF,
                uint32 aTruncateFlag = 1);

        OSCL_IMPORT_REF static PVMFStatus CreateKVPForByteArrayValue(PvmiKvp& aKeyVal,
                const char* aKeyTypeString,
                uint8* aValue,
                uint32 aValueLen,
                char* aMiscKeyParam = NULL,
                uint32 aMaxSize = 0xFFFFFFFF);

        OSCL_IMPORT_REF static PVMFStatus CreateKVPForUInt32Value(PvmiKvp& aKeyVal,
                const char* aKeyTypeString,
                uint32& aValueUInt32,
                char* aMiscKeyParam = NULL);

        OSCL_IMPORT_REF static PVMFStatus CreateKVPForFloatValue(PvmiKvp& aKeyVal,
                const char* aKeyTypeString,
                float& aValueFloat,
                char* aMiscKeyParam = NULL);

        OSCL_IMPORT_REF static PVMFStatus CreateKVPForBoolValue(PvmiKvp& aKeyVal,
                const char* aKeyTypeString,
                bool& aValueBool,
                char* aMiscKeyParam = NULL);

        OSCL_IMPORT_REF static PVMFStatus CreateKVPForKSVValue(PvmiKvp& aKeyVal,
                const char* aKeyTypeString,
                OsclAny* aValue,
                char* aMiscKeyParam = NULL);

};

#endif //PVMI_KVP_UTIL_H_INCLUDED

