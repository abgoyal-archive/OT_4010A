

#ifndef PVMP3_MPEG2_GET_SCALE_DATA_H
#define PVMP3_MPEG2_GET_SCALE_DATA_H


#include "pvmp3_dec_defs.h"
#include "pvmp3_audio_type_defs.h"
#include "s_mp3bits.h"








#ifdef __cplusplus
extern "C"
{
#endif

    void pvmp3_mpeg2_get_scale_data(mp3SideInfo  *si,
    int32         gr,
    int32         ch,
    mp3Header    *info,
    uint32       *scalefac_buffer,
    uint32       *scalefac_IIP_buffer,
    tmp3Bits     *pMainData);

#ifdef __cplusplus
}
#endif

#endif

