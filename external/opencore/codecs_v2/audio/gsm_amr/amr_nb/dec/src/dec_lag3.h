

#ifndef dec_lag3_h
#define dec_lag3_h "$Id $"


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

    void Dec_lag3(Word16 index,     /* i : received pitch index                 */
    Word16 T0_min,    /* i : minimum of search range              */
    Word16 T0_max,    /* i : maximum of search range              */
    Word16 i_subfr,   /* i : subframe flag                        */
    Word16 T0_prev,   /* i : integer pitch delay of last subframe
                                       used in 2nd and 4th subframes        */
    Word16 * T0,      /* o : integer part of pitch lag            */
    Word16 * T0_frac, /* o : fractional part of pitch lag         */
    Word16 flag4,     /* i : flag for encoding with 4 bits        */
    Flag   * pOverflow  /* o : Flag set when overflow occurs      */
                 );

    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _DEC_LAG_3_H_ */

