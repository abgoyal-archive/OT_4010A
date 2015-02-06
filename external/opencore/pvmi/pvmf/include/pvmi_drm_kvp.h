

#ifndef PVMI_DRM_KVP_H_INCLUDED
#define PVMI_DRM_KVP_H_INCLUDED

#define PVMF_DRM_INFO_IS_PROTECTED_QUERY "drm/is-protected"
#define PVMF_DRM_INFO_IS_PROTECTED_VALUE "drm/is-protected;valtype=bool"

#define PVMF_DRM_INFO_IS_LICENSE_AVAILABLE_QUERY "drm/is-license-available"
#define PVMF_DRM_INFO_IS_LICENSE_AVAILABLE_VALUE "drm/is-license-available;valtype=bool"

#define PVMF_DRM_INFO_IS_FORWARD_LOCKED_QUERY "drm/is-forward-locked"
#define PVMF_DRM_INFO_IS_FORWARD_LOCKED_VALUE "drm/is-forward-locked;valtype=bool"

#define PVMF_DRM_INFO_CAN_USE_AS_RINGTONE_QUERY "drm/can-use-as-ringtone"
#define PVMF_DRM_INFO_CAN_USE_AS_RINGTONE_VALUE "drm/can-use-as-ringtone;valtype=bool"

#define PVMF_DRM_INFO_LICENSE_TYPE_QUERY "drm/license-type"
#define PVMF_DRM_INFO_LICENSE_TYPE_VALUE "drm/license-type;valtype=char*"

#define PVMF_DRM_INFO_LICENSE_COUNT_QUERY "drm/num-counts"
#define PVMF_DRM_INFO_LICENSE_COUNT_VALUE "drm/num-counts;valtype=uint32"


#define PVMF_DRM_INFO_LICENSE_START_TIME_QUERY "drm/license-start"
#define PVMF_DRM_INFO_LICENSE_START_TIME_VALUE "drm/license-start;valtype=char*"

#define PVMF_DRM_INFO_LICENSE_EXPIRATION_TIME_QUERY "drm/license-expiry"
#define PVMF_DRM_INFO_LICENSE_EXPIRATION_TIME_VALUE "drm/license-expiry;valtype=char*"

#define PVMF_DRM_INFO_LICENSE_START_TIME_VALUE_FORMAT_MS_FILETIME "drm/license-start;valtype=pUnit64_value;format=ms-filetime"

#define PVMF_DRM_INFO_LICENSE_EXPIRATION_TIME_VALUE_FORMAT_MS_FILETIME "drm/license-expiry;valtype=pUnit64_value;format=ms-filetime"

#define PVMF_DRM_INFO_LICENSE_DURATION_SINCE_FIRST_USE_QUERY "drm/duration"
#define PVMF_DRM_INFO_LICENSE_DURATION_SINCE_FIRST_USE_VALUE "drm/duration;valtype=uint32"

#define PVMF_DRM_INFO_LICENSE_ISSUER_QUERY "dla/license-issuer"
#define PVMF_DRM_INFO_LICENSE_ISSUER_VALUE "dla/license-issuer;valtype=char*"

#define PVMF_DRM_INFO_REDIRECT_TIMES_QUERY "dla/num-redirect"
#define PVMF_DRM_INFO_REDIRECT_TIMES_VALUE "dla/num-redirect;valtype=uint32"

#define PVMF_DRM_INFO_ENVELOPE_DATA_SIZE_QUERY "drm/envelope-data-size"
#define PVMF_DRM_INFO_ENVELOPE_DATA_SIZE_VALUE "drm/envelope-data-size;valtype=uint32"
#define PVMF_DRM_INFO_ENVELOPE_DATA_QUERY "drm/envelope-data"
#define PVMF_DRM_INFO_ENVELOPE_DATA_VALUE "drm/envelope-data;valtype=uint8*"

#endif // PVMI_DRM_KVP_H_INCLUDED




