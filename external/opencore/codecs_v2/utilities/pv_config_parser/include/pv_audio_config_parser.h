
#ifndef PV_AUDIO_CONFIG_PARSER_H_INCLUDED
#define PV_AUDIO_CONFIG_PARSER_H_INCLUDED

#include "oscl_base.h"
#include "oscl_types.h"
#include "pvmf_format_type.h"

typedef struct
{
    uint8* inPtr;
    uint32 inBytes;
    PVMFFormatType iMimeType;
} pvAudioConfigParserInputs;


typedef struct
{
    uint16 Channels;
    uint16 BitsPerSample;
    uint32 SamplesPerSec;
} pvAudioConfigParserOutputs;


OSCL_IMPORT_REF int32 pv_audio_config_parser(pvAudioConfigParserInputs *aInputs, pvAudioConfigParserOutputs *aOutputs);


#endif //PV_AUDIO_CONFIG_PARSER_H_INCLUDED


