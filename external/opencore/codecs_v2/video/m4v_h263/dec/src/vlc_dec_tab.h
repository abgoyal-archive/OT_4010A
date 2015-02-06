

#ifndef vlc_dec_tab_H
#define vlc_dec_tab_H

#include "mp4def.h"





#ifdef __cplusplus
extern "C"
{
#endif

    extern const VLCshorttab PV_TMNMVtab0[];

    extern const VLCshorttab PV_TMNMVtab1[];

    extern const VLCshorttab PV_TMNMVtab2[];

    extern const VLCshorttab PV_MCBPCtab[];

#ifdef PV_ANNEX_IJKT_SUPPORT
    extern const VLCshorttab PV_MCBPCtab1[];
#endif
    extern const VLCshorttab PV_MCBPCtabintra[];

    /* Table for separate mode MCBPC, for coding DQUANT-flag and CBPC */

    extern const VLCshorttab MCBPCtab_sep[32];

    extern const VLCshorttab PV_CBPYtab[48];

    extern const VLCshorttab CBPYtab2[16];

    extern const VLCshorttab CBPYtab3[64];

    extern const VLCtab2 PV_DCT3Dtab0[];


    extern const VLCtab2 PV_DCT3Dtab1[];


    extern const VLCtab2 PV_DCT3Dtab2[];

    /* New tables for Intra luminance blocks */

    extern const VLCtab2 PV_DCT3Dtab3[];

    extern const VLCtab2 PV_DCT3Dtab4[];

    extern const VLCtab2 PV_DCT3Dtab5[];
#ifdef PV_ANNEX_IJKT_SUPPORT
    /* Annex I tables */
    extern const VLCtab2 PV_DCT3Dtab6[];

    extern const VLCtab2 PV_DCT3Dtab7[];

    extern const VLCtab2 PV_DCT3Dtab8[];
#endif
    /* RVLC tables */
    extern const int ptrRvlcTab[];

    extern const VLCtab2 RvlcDCTtabIntra[];

    extern const VLCtab2 RvlcDCTtabInter[];

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


    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#endif

#ifdef __cplusplus
}
#endif








