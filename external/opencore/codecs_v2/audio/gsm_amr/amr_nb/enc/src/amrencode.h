

#ifndef _AMRENCODE_H_
#define _AMRENCODE_H_

#include "typedef.h"
#include "mode.h"
#include "frame_type_3gpp.h"

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
#define NUM_AMRSID_TXMODE_BITS     3
#define AMRSID_TXMODE_BIT_OFFSET   36
#define AMRSID_TXTYPE_BIT_OFFSET   35

    /* Output format types */
#define AMR_TX_WMF 0
#define AMR_TX_IF2 1
#define AMR_TX_ETS 2

    /*----------------------------------------------------------------------------
    ; EXTERNAL VARIABLES REFERENCES
    ; [Declare variables used in this module but defined elsewhere]
    ----------------------------------------------------------------------------*/
    extern const Word16 WmfEncBytesPerFrame[];
    extern const Word16 If2EncBytesPerFrame[];

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
    ; [List function prototypes here]
    ----------------------------------------------------------------------------*/
    Word16 AMREncodeInit(
        void **pEncStructure,
        void **pSidSyncStructure,
        Flag dtx_enable);

    Word16 AMREncodeReset(
        void *pEncStructure,
        void *pSidSyncStructure);

    void AMREncodeExit(
        void **pEncStructure,
        void **pSidSyncStructure);

    Word16 AMREncode(
        void *pEncState,
        void *pSidSyncState,
        enum Mode mode,
        Word16 *pEncInput,
        UWord8 *pEncOutput,
        enum Frame_Type_3GPP *p3gpp_frame_type,
        Word16 output_format
    );

#ifdef __cplusplus
}
#endif

#endif  /* _AMRENCODE_H_ */


