
#ifndef idct_h
#define idct_h

#include "mp4dec_lib.h"


#define INTEGER_IDCT

#ifdef FAST_IDCT
#ifndef INTEGER_IDCT
#define INTEGER_IDCT
#endif
#endif

#ifdef FAST_IDCT
#ifdef __cplusplus
extern "C"
{
#endif
    void idctrow0(int16 *blk, uint8 *pred, uint8 *dst, int width);
    void idctrow1(int16 *blk, uint8 *pred, uint8 *dst, int width);
    void idctrow2(int16 *blk, uint8 *pred, uint8 *dst, int width);
    void idctrow3(int16 *blk, uint8 *pred, uint8 *dst, int width);
    void idctrow4(int16 *blk, uint8 *pred, uint8 *dst, int width);
    void idctcol0(int16 *blk);
    void idctcol1(int16 *blk);
    void idctcol2(int16 *blk);
    void idctcol3(int16 *blk);
    void idctcol4(int16 *blk);

    void idctrow0_intra(int16 *blk, PIXEL *comp, int width);
    void idctrow1_intra(int16 *blk, PIXEL *comp, int width);
    void idctrow2_intra(int16 *blk, PIXEL *comp, int width);
    void idctrow3_intra(int16 *blk, PIXEL *comp, int width);
    void idctrow4_intra(int16 *blk, PIXEL *comp, int width);
#ifdef __cplusplus
}
#endif
#endif

/* this code assumes ">>" to be a two's-complement arithmetic */
/* right shift: (-2)>>1 == -1 , (-3)>>1 == -2                 */

/* a positive real constant is converted to an integer scaled by 2048 */
/* or equivalent to left shift by 11 */

#define W1 2841                 /* 2048*sqrt(2)*cos(1*pi/16) */
#define W2 2676                 /* 2048*sqrt(2)*cos(2*pi/16) */
#define W3 2408                 /* 2048*sqrt(2)*cos(3*pi/16) */
#define W5 1609                 /* 2048*sqrt(2)*cos(5*pi/16) */
#define W6 1108                 /* 2048*sqrt(2)*cos(6*pi/16) */
#define W7 565                  /* 2048*sqrt(2)*cos(7*pi/16) */
#define W1mW7 2276
#define W1pW7 3406
#define W5mW3 -799
#define mW3mW5 -4017
#define mW2mW6 -3784
#define W2mW6 1568

/* left shift by 11 is to maintain the accuracy of the decimal point */
/* for the transform coefficients (W1,...W7) */






#endif


