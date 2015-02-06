
#ifndef PV_VIDEO_CONFIG_PARSER_H_INCLUDED
#define PV_VIDEO_CONFIG_PARSER_H_INCLUDED

#include "oscl_base.h"
#include "oscl_types.h"
#include "pvmf_format_type.h"

typedef struct
{
    uint8 *inPtr;
    uint32 inBytes;
    PVMFFormatType iMimeType;
} pvVideoConfigParserInputs;

typedef struct
{
    uint32 width;
    uint32 height;
    uint32 profile;
    uint32 level;
} pvVideoConfigParserOutputs;

OSCL_IMPORT_REF int16 pv_video_config_parser(pvVideoConfigParserInputs *aInputs, pvVideoConfigParserOutputs *aOutputs);


#endif //PV_VIDEO_CONFIG_PARSER_H_INCLUDED


