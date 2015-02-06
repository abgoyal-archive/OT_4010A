

#ifndef S_MC_INFO_H
#define S_MC_INFO_H

#include "pv_audio_type_defs.h"
#include "e_rawbitstreamconst.h"
#include "s_ch_info.h"
#include "chans.h"
#include "e_tmp4audioobjecttype.h"







typedef struct
{
    Int nch;        /* total number of audio channels */
    Int nfsce;      /* number of front SCE's pror to first front CPE */
    Int nfch;       /* number of front channels */
    Int nsch;       /* number of side channels */
    Int nbch;       /* number of back channels */
    Int nlch;       /* number of lfe channels */
    Int ncch;       /* number of valid coupling channels */
    tMP4AudioObjectType audioObjectType;    /* Should eventually be called object */
    Int sampling_rate_idx;

    Int implicit_channeling;
    Int  upsamplingFactor;
#ifdef AAC_PLUS
    bool bDownSampledSbr;
    Int HE_AAC_level;
#endif
    /* All AAC content should be aware of these flag */
    /*  AAC+ content Flag */
    Int sbrPresentFlag;
    /*  Enhanced AAC+ content Flag */
    Int psPresentFlag;
    tMP4AudioObjectType ExtendedAudioObjectType;    /* Should eventually be called object */

    Ch_Info ch_info[Chans];
} MC_Info;


#endif

