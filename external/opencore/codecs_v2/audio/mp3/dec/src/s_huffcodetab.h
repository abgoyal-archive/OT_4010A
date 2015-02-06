
#ifndef  S_HUFFCODETAB_H
#define  S_HUFFCODETAB_H

#include "pvmp3_audio_type_defs.h"


#define HUFF_TBL    34





struct huffcodetab
{
    uint32 linbits;                  /*number of linbits  */
    uint16(*pdec_huff_tab)(tmp3Bits *);
};



#endif

