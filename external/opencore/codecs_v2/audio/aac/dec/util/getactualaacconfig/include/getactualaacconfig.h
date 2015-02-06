


#ifndef GETACTUALAACCONFIG_H
#define GETACTUALAACCONFIG_H

#include "oscl_base.h"

OSCL_IMPORT_REF  int32 GetActualAacConfig(uint8* aConfigHeader,
        uint8* aAudioObjectType,
        int32* aConfigHeaderSize,
        uint8* SamplingRateIndex,
        uint32* NumChannels);


#endif
