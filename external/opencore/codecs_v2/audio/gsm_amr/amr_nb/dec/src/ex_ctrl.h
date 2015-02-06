

#ifndef ex_ctrl_h
#define ex_ctrl_h "$Id $"

#include "typedef.h"
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
#define L_ENERGYHIST 60
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
    /*
     *  Function    : Ex_ctrl
     *  Purpose     : Charaterice synthesis speech and detect background noise
     *  Returns     : background noise decision; 0 = bgn, 1 = no bgn
     */
    Word16 Ex_ctrl(Word16 excitation[],    /*i/o: Current subframe excitation   */
    Word16 excEnergy,      /* i : Exc. Energy, sqrt(totEx*totEx)*/
    Word16 exEnergyHist[], /* i : History of subframe energies  */
    Word16 voicedHangover, /* i : # of fr. after last voiced fr.*/
    Word16 prevBFI,        /* i : Set i previous BFI            */
    Word16 carefulFlag,    /* i : Restrict dymamic in scaling   */
    Flag   *pOverflow
                  );

    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _ex_ctrl_h_ */







