
#ifndef post_proc_H
#define post_proc_H

#include    "mp4dec_lib.h"


#define UPDATE_PV_MAXPV_MIN(p,max,min) if ((p) > max) max=(p); else if ((p) < min) min = (p);

#define     INDEX(x,thr)    (((x)>=thr)?1:0)
#define     BLKSIZE     8
#define     MBSIZE      16
#define     DERING_THR  16

/* version for fast Deblock filtering*/
#define     KTh     4  /*threshold for soft filtering*/
#define     KThH    4  /*threshold for hard filtering */

#define     NoMMX






#endif


