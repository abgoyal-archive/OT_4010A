

#ifndef _POST_PRO_H_
#define _POST_PRO_H_
#define post_pro_h "$Id $"

#include "typedef.h"

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
        Word16 y2_hi;
        Word16 y2_lo;
        Word16 y1_hi;
        Word16 y1_lo;
        Word16 x0;
        Word16 x1;
    } Post_ProcessState;

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; [List function prototypes here]
    ----------------------------------------------------------------------------*/

    Word16 Post_Process_reset(Post_ProcessState *st);
    /* reset of Post processing state (i.e. set state memory to zero)
       returns 0 on success
     */
    void Post_Process_exit(Post_ProcessState **st);
    /* de-initialize Post processing state (i.e. free status struct)
       stores NULL in *st
     */

    void Post_Process(
        Post_ProcessState *st,  /* i/o : post process state                   */
        Word16 signal[],        /* i/o : signal                               */
        Word16 lg,              /* i   : lenght of signal                     */
        Flag *pOverflow
    );

#ifdef __cplusplus
}
#endif

#endif  /* _POST_PRO_H_ */





