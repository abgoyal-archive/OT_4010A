
#ifndef _GSMAMR_DEC_H_
#define _GSMAMR_DEC_H_


#include "gsm_amr_typedefs.h"
#include "pvamrnbdecoder_api.h"
/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

    /*----------------------------------------------------------------------------
    ; MACROS
    ----------------------------------------------------------------------------*/


    /*----------------------------------------------------------------------------
    ; DEFINES
    ----------------------------------------------------------------------------*/
    /* Number of 13-bit linear PCM samples per 20 ms frame */
    /* L_FRAME = (8 kHz) * (20 msec) = 160 samples         */
#define L_FRAME     160

    /*----------------------------------------------------------------------------
    ; EXTERNAL VARIABLES REFERENCES
    ----------------------------------------------------------------------------*/


    /*----------------------------------------------------------------------------
    ; SIMPLE TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; ENUMERATED TYPEDEF'S
    ----------------------------------------------------------------------------*/
    enum Frame_Type_3GPP
    {
        AMR_475 = 0,        /* 4.75 kbps    */
        AMR_515,            /* 5.15 kbps    */
        AMR_59,             /* 5.9 kbps     */
        AMR_67,             /* 6.7 kbps     */
        AMR_74,             /* 7.4 kbps     */
        AMR_795,            /* 7.95 kbps    */
        AMR_102,            /* 10.2 kbps    */
        AMR_122,            /* 12.2 kbps    */
        AMR_SID,            /* GSM AMR DTX  */
        GSM_EFR_SID,        /* GSM EFR DTX  */
        TDMA_EFR_SID,       /* TDMA EFR DTX */
        PDC_EFR_SID,        /* PDC EFR DTX  */
        FOR_FUTURE_USE1,    /* Unused 1     */
        FOR_FUTURE_USE2,    /* Unused 2     */
        FOR_FUTURE_USE3,    /* Unused 3     */
        AMR_NO_DATA
    };      /* No data      */

    /*----------------------------------------------------------------------------
    ; STRUCTURES TYPEDEF'S
    ----------------------------------------------------------------------------*/


    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ----------------------------------------------------------------------------*/
    /*
     * This function allocates memory for filter structure and initializes state
     * memory used by the GSM AMR decoder. This function returns zero. It will
     * return negative one if there is an error.
     */
    Word16 GSMInitDecode(void **state_data,
                         Word8 *id);

    /*
     * AMRDecode steps into the part of the library that decodes the raw data
     * speech bits for the decoding process. It returns the address offset of
     * the next frame to be decoded.
     */
    Word16 AMRDecode(
        void                      *state_data,
        enum Frame_Type_3GPP      frame_type,
        UWord8                    *speech_bits_ptr,
        Word16                    *raw_pcm_buffer,
        Word16                    input_format
    );

    /*
     * This function resets the state memory used by the GSM AMR decoder. This
     * function returns zero. It will return negative one if there is an error.
     */
    Word16 Speech_Decode_Frame_reset(void *state_data);

    /*
     * This function frees up the memory used for the state memory of the
     * GSM AMR decoder.
     */
    void GSMDecodeFrameExit(void **state_data);


#ifdef __cplusplus
}
#endif

#endif  /* _GSMAMR_DEC_H_ */

