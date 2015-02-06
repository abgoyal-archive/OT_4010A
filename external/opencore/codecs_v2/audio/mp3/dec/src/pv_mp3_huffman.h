

#ifndef PV_MP3_HUFFMAN_H
#define PV_MP3_HUFFMAN_H



#include "pvmp3_audio_type_defs.h"
#include "s_mp3bits.h"
#include "s_tmp3dec_file.h"






#ifdef __cplusplus
extern "C"
{
#endif

    int32 pvmp3_huffman_parsing(int32 is[SUBBANDS_NUMBER*FILTERBANK_BANDS],
    granuleInfo *grInfo,
    tmp3dec_file   *pVars,
    int32 part2_start,
    mp3Header *info);


    void pvmp3_huffman_quad_decoding(struct huffcodetab *h,
                                     int32 *is,
                                     tmp3Bits *pMainData);

    void pvmp3_huffman_pair_decoding(struct huffcodetab *h,
                                     int32 *is,
                                     tmp3Bits *pMainData);


    void pvmp3_huffman_pair_decoding_linbits(struct huffcodetab *h,
            int32 *is,
            tmp3Bits *pMainData);

#ifdef __cplusplus
}
#endif


#endif



