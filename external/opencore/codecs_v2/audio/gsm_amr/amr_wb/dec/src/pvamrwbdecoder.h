

#ifndef _PVAMRWBDECODER_H
#define _PVAMRWBDECODER_H

#include    "oscl_base.h"    /* Basic data types used within the lib */
#include    "pvamrwbdecoder_api.h"
#include    "pvgsmamrdecoderinterface.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /*----------------------------------------------------------------------------
    ; MACROS
    ; Define module specific macros here
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; DEFINES
    ; Include all pre-processor statements here.
    ----------------------------------------------------------------------------*/



    /*----------------------------------------------------------------------------
    ; EXTERNAL VARIABLES REFERENCES
    ; Declare variables used in this module but defined elsewhere
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; SIMPLE TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; ENUMERATED TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; STRUCTURES TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/

    void pvDecoder_AmrWb_Init(void **spd_state, void *st, int16 ** ScratchMem);

    int32 pvDecoder_AmrWb(
        int16 mode,                          /* input : used mode             */
        int16 prms[],                        /* input : parameter vector      */
        int16 synth16k[],                    /* output: synthesis speech      */
        int16 * frame_length,                /* output:  lenght of the frame  */
        void *spd_state,                     /* i/o   : State structure       */
        int16 frame_type,                    /* input : received frame type   */
        int16 ScratchMem[]
    );

    void pvDecoder_AmrWb_Reset(void *st, int16 reset_all);

    int16 pvDecoder_AmrWb_homing_frame_test(int16 input_frame[], int16 mode);

    int16 pvDecoder_AmrWb_homing_frame_test_first(int16 input_frame[], int16 mode);

    int32 pvDecoder_AmrWbMemRequirements();

    void mime_unsorting(uint8 packet[],
                        int16 compressed_data[],
                        int16 *frame_type,
                        int16 *mode,
                        uint8 q,
                        RX_State *st);


    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif


#endif  /* PVMP4AUDIODECODER_API_H */


