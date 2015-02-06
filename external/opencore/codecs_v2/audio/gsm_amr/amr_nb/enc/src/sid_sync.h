

#ifndef _SID_SYNC_H_
#define _SID_SYNC_H_

#include "typedef.h"
#include "mode.h"
#include "frame.h"

/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

    /*----------------------------------------------------------------------------
    ; MACROS
    ; [Define module specific macros here]
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; DEFINES
    ; [Include all pre-processor statements here.]
    ----------------------------------------------------------------------------*/
#define sid_sync_h "$Id $"

    /*----------------------------------------------------------------------------
    ; EXTERNAL VARIABLES REFERENCES
    ; [Declare variables used in this module but defined elsewhere]
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
    typedef struct
    {
        Word16 sid_update_rate;  /* Send SID Update every sid_update_rate frame */
        Word16 sid_update_counter; /* Number of frames since last SID          */
        Word16 sid_handover_debt;  /* Number of extra SID_UPD frames to schedule*/
        enum TXFrameType prev_ft;
    } sid_syncState;

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; [List function prototypes here]
    ----------------------------------------------------------------------------*/
    /* initialize one instance of the sid_sync module
       Stores pointer to state struct in *st. This pointer has to
       be passed to sid_sync in each call.
       returns 0 on success
     */
    Word16 sid_sync_init(void **st);

    /* reset of sid_sync module (i.e. set state memory to zero)
       returns 0 on success
     */
    Word16 sid_sync_reset(void *st);

    /* de-initialize sid_sync module (i.e. free status struct)
       stores NULL in *st
     */
    void sid_sync_exit(void **st);

    /*  update handover debt
        debtFrames extra SID_UPD are scheduled .
        to update remote decoder CNI states, right after an handover.
        (primarily for use on MS UL side )
    */
    void sid_sync_set_handover_debt(sid_syncState *st,  /* i/o: sid_sync state  */
                                    Word16 debtFrames);

    /* To ensure that the mode only switches to a neighbouring mode */
    void sid_sync(void *st ,
                  enum Mode mode,
                  enum TXFrameType *tx_frame_type);

#ifdef __cplusplus
}
#endif

#endif  /* _SID_SYNC_H_ */
