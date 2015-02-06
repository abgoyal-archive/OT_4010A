

#ifndef S_CH_INFO_H
#define S_CH_INFO_H

#include "pv_audio_type_defs.h"







typedef struct
{
//    Int present;    /* channel present */
    Int tag;        /* element tag */
    Int cpe;        /* 0 if single channel, 1 if channel pair */
//    Int common_window;  /* 1 if common window for cpe */
//    Int ch_is_left; /* 1 if left channel of cpe */
//    Int paired_ch;  /* index of paired channel in cpe */
//    Int widx;       /* window element index for this channel */
    Int is_present; /* intensity stereo is used */
    Int ncch;       /* number of coupling channels for this ch */
    /* #if (CChans > 0) */
    /*    int cch[CChans];*/    /* coupling channel idx */
    /*    int cc_dom[CChans];*/ /* coupling channel domain */
    /*    int cc_ind[CChans];*/ /* independently switched coupling channel flag */
    /* #endif */
    Char *fext;     /* filename extension */

} Ch_Info;


#endif

