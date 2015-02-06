

#ifndef pre_big_h
#define pre_big_h "$Id $"

#include "typedef.h"
#include "mode.h"
#include "cnst.h"

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

    void pre_big(
        enum Mode mode,            /* i  : coder mode                             */
        const Word16 gamma1[],     /* i  : spectral exp. factor 1                 */
        const Word16 gamma1_12k2[],/* i  : spectral exp. factor 1 for EFR         */
        const Word16 gamma2[],     /* i  : spectral exp. factor 2                 */
        Word16 A_t[],              /* i  : A(z) unquantized, for 4 subframes, Q12 */
        Word16 frameOffset,        /* i  : Start position in speech vector,   Q0  */
        Word16 speech[],           /* i  : speech,                            Q0  */
        Word16 mem_w[],            /* i/o: synthesis filter memory state,     Q0  */
        Word16 wsp[],              /* o  : weighted speech                    Q0  */
        Flag   *pOverflow          /* o  : overflow indicator                     */
    );

    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _H_ */

