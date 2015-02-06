

#ifndef DCT64_H
#define DCT64_H



#include "pv_audio_type_defs.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /*----------------------------------------------------------------------------
    ; EXTERNAL VARIABLES REFERENCES
    ----------------------------------------------------------------------------*/
    extern const Int32 CosTable_48[48];

    /*----------------------------------------------------------------------------
    ; DEFINES AND SIMPLE TYPEDEF'S
    ----------------------------------------------------------------------------*/

    void pv_split_LC(Int32 *vector,
                     Int32 *temp_o);


#ifdef HQ_SBR

    void dct_64(Int32 vec[], Int32 *scratch_mem);

#endif

    void pv_split(Int32 *temp_o);

    void pv_split_z(Int32 *vector);

    void pv_merge_in_place_N32(Int32 vec[]);

#ifdef __cplusplus
}
#endif

#endif  /* DCT64_H */
