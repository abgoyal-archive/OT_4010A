

#ifndef q_plsf_h
#define q_plsf_h "$Id $"

#include "typedef.h"
#include "cnst.h"
#include "mode.h"

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
#define MR795_1_SIZE  512
#define PAST_RQ_INIT_SIZE 8

#define DICO1_SIZE  256
#define DICO2_SIZE  512
#define DICO3_SIZE  512

#define DICO1_5_SIZE  128
#define DICO2_5_SIZE  256
#define DICO3_5_SIZE  256
#define DICO4_5_SIZE  256
#define DICO5_5_SIZE  64

#define MR515_3_SIZE  128

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
        Word16 past_rq[M];    /* Past quantized prediction error, Q15 */

    } Q_plsfState;

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/

    Word16 Q_plsf_init(Q_plsfState **st);
    /* initialize one instance of the state.
       Stores pointer to filter status struct in *st. This pointer has to
       be passed to Q_plsf_5 / Q_plsf_3 in each call.
       returns 0 on success
     */

    Word16 Q_plsf_reset(Q_plsfState *st);
    /* reset of state (i.e. set state memory to zero)
       returns 0 on success
     */

    void Q_plsf_exit(Q_plsfState **st);
    /* de-initialize state (i.e. free status struct)
       stores NULL in *st
     */

    void Q_plsf_3(
        Q_plsfState *st,    /* i/o: state struct                             */
        enum Mode mode,     /* i  : coder mode                               */
        Word16 *lsp1,       /* i  : 1st LSP vector                      Q15  */
        Word16 *lsp1_q,     /* o  : quantized 1st LSP vector            Q15  */
        Word16 *indice,     /* o  : quantization indices of 3 vectors   Q0   */
        Word16 *pred_init_i,/* o  : init index for MA prediction in DTX mode */
        Flag  *pOverflow    /* o : Flag set when overflow occurs             */
    );

    void Q_plsf_5(
        Q_plsfState *st,
        Word16 *lsp1,      /* i  : 1st LSP vector,                     Q15 */
        Word16 *lsp2,      /* i  : 2nd LSP vector,                     Q15 */
        Word16 *lsp1_q,    /* o  : quantized 1st LSP vector,           Q15 */
        Word16 *lsp2_q,    /* o  : quantized 2nd LSP vector,           Q15 */
        Word16 *indice,    /* o  : quantization indices of 5 matrices, Q0  */
        Flag  *pOverflow   /* o : Flag set when overflow occurs            */
    );
    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _Q_PLSF_H_ */

