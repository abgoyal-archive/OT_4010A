
#ifndef sp_dec_h
#define sp_dec_h "$Id $"

#include "typedef.h"
#include "cnst.h"
#include "dec_amr.h"
#include "pstfilt.h"
#include "post_pro.h"
#include "mode.h"

typedef struct
{
    Decoder_amrState  decoder_amrState;
    Post_FilterState  post_state;
    Post_ProcessState postHP_state;
    enum Mode prev_mode;
} Speech_Decode_FrameState;


#if defined(__cplusplus)
extern "C"
{
#endif
    Word16 GSMInitDecode(void **state_data,
    Word8 *id);
    /* initialize one instance of the speech decoder
       Stores pointer to filter status struct in *st. This pointer has to
       be passed to Speech_Decode_Frame in each call.
       returns 0 on success
     */

    Word16 Speech_Decode_Frame_reset(void *state_data);
    /* reset speech decoder (i.e. set state memory to zero)
       returns 0 on success
     */

    void GSMDecodeFrameExit(void **state_data);
    /* de-initialize speech decoder (i.e. free status struct)
       stores NULL in *s
     */

    void GSMFrameDecode(
        Speech_Decode_FrameState *st, /* io: post filter states                */
        enum Mode mode,               /* i : AMR mode                          */
        Word16 *serial,               /* i : serial bit stream                 */
        enum RXFrameType frame_type,  /* i : Frame type                        */
        Word16 *synth                 /* o : synthesis speech (postfiltered    */
        /*     output)                           */
    );
    /*    return 0 on success
     */
#if defined(__cplusplus)
}
#endif
#endif
