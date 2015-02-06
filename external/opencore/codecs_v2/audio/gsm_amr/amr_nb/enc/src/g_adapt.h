

#ifndef g_adapt_h
#define g_adapt_h "$Id $"

#include "typedef.h"

/*--------------------------------------------------------------------------*/
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
#define LTPG_MEM_SIZE 5 /* number of stored past LTP coding gains + 1 */

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
    typedef struct
    {
        Word16 onset;                   /* onset state,                   Q0  */
        Word16 prev_alpha;              /* previous adaptor output,       Q15 */
        Word16 prev_gc;                 /* previous code gain,            Q1  */

        Word16 ltpg_mem[LTPG_MEM_SIZE]; /* LTP coding gain history,       Q13 */
        /* (ltpg_mem[0] not used for history) */
    } GainAdaptState;

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/
    Word16 gain_adapt_init(GainAdaptState **st);
    /* initialize one instance of the gain adaptor
       Stores pointer to state struct in *st. This pointer has to
       be passed to gain_adapt and gain_adapt_update in each call.
       returns 0 on success
     */

    Word16 gain_adapt_reset(GainAdaptState *st);
    /* reset of gain adaptor state (i.e. set state memory to zero)
       returns 0 on success
     */

    void gain_adapt_exit(GainAdaptState **st);
    /* de-initialize gain adaptor state (i.e. free state struct)
       stores NULL in *st
     */

    /*************************************************************************
     *
     *  Function:   gain_adapt()
     *  Purpose:    calculate pitch/codebook gain adaptation factor alpha
     *              (and update the adaptor state)
     *
     **************************************************************************
     */
    void gain_adapt(
        GainAdaptState *st,  /* i  : state struct                  */
        Word16 ltpg,         /* i  : ltp coding gain (log2()), Q   */
        Word16 gain_cod,     /* i  : code gain,                Q13 */
        Word16 *alpha,       /* o  : gain adaptation factor,   Q15 */
        Flag   *pOverflow    /* o  : overflow indicator            */
    );

    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _H_ */


