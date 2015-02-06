
#ifndef motion_comp_h
#define motion_comp_h

#include "mp4dec_lib.h"


/* CBP Mask defines used in chrominance prediction */
#define CBP_MASK_CHROMA_BLK4    0x2
#define CBP_MASK_CHROMA_BLK5    0x1

/* CBP Mask defines used in luminance prediction (MODE_INTER4V) */
#define CBP_MASK_BLK0_MODE_INTER4V  0x20
#define CBP_MASK_BLK1_MODE_INTER4V  0x10
#define CBP_MASK_BLK2_MODE_INTER4V  0x08
#define CBP_MASK_BLK3_MODE_INTER4V  0x04

/* CBP Mask defines used in luminance prediction (MODE_INTER or MODE_INTER_Q) */
#define CBP_MASK_MB_MODE_INTER  0x3c

#ifdef __cplusplus
extern "C"
{
#endif

#define CLIP_RESULT(x)      if(x & -256){x = 0xFF & (~(x>>31));}
#define ADD_AND_CLIP1(x)    x += (pred_word&0xFF); CLIP_RESULT(x);
#define ADD_AND_CLIP2(x)    x += ((pred_word>>8)&0xFF); CLIP_RESULT(x);
#define ADD_AND_CLIP3(x)    x += ((pred_word>>16)&0xFF); CLIP_RESULT(x);
#define ADD_AND_CLIP4(x)    x += ((pred_word>>24)&0xFF); CLIP_RESULT(x);

#define ADD_AND_CLIP(x,y)    {  x9 = ~(x>>8); \
                            if(x9!=-1){ \
                                x9 = ((uint32)x9)>>24; \
                                y = x9|(y<<8); \
                            } \
                            else \
                            {    \
                                y =  x|(y<<8); \
                            } \
                            }


    static int (*const GetPredAdvBTable[2][2])(uint8*, uint8*, int, int) =
    {
        {&GetPredAdvancedBy0x0, &GetPredAdvancedBy0x1},
        {&GetPredAdvancedBy1x0, &GetPredAdvancedBy1x1}
    };

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



