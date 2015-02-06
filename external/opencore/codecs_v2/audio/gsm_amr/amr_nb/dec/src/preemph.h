

#ifndef preemph_h
#define preemph_h "$Id $"



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
    typedef struct
    {
        Word16 mem_pre;          /* filter state */
    } preemphasisState;

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/

    Word16 preemphasis_reset(preemphasisState *st);
    /* reset of preemphasis filter (i.e. set state memory to zero)
       returns 0 on success
     */
    void preemphasis_exit(preemphasisState **st);
    /* de-initialize preemphasis filter (i.e. free status struct)
       stores NULL in *st
     */

    void preemphasis(
        preemphasisState *st, /* (i/o): preemphasis filter state                */
        Word16 *signal,    /* (i/o): input signal overwritten by the output     */
        Word16 g,          /* (i)  : preemphasis coefficient                    */
        Word16 L,          /* (i)  : size of filtering                          */
        Flag   *pOverflow  /* (o)  : overflow indicator                         */
    );

    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* preemph_H_ */


