

#ifndef PVMP3_GET_MAIN_DATA_SIZE_H
#define PVMP3_GET_MAIN_DATA_SIZE_H


#include "pvmp3_dec_defs.h"
#include "pvmp3_audio_type_defs.h"
#include "s_tmp3dec_file.h"








#ifdef __cplusplus
extern "C"
{
#endif

    int32 pvmp3_get_main_data_size(mp3Header *info,
    tmp3dec_file  *pVars);

#ifdef __cplusplus
}
#endif

#endif


