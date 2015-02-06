

#ifndef _AVCDEC_INT_H_
#define _AVCDEC_INT_H_

#include "avcint_common.h"
#include "avcdec_api.h"


typedef struct tagDecBitstream
{
    uint8 *bitstreamBuffer; /* pointer to buffer memory   */
    int nal_size;       /* size of the current NAL unit */
    int data_end_pos;  /* bitstreamBuffer size in bytes */
    int read_pos;       /* next position to read from bitstreamBuffer  */
    uint curr_word; /* byte-swapped (MSB left) current word read from buffer */
    int bit_left;      /* number of bit left in current_word */
    uint next_word;     /* in case for old data in previous buffer hasn't been flushed. */
    int incnt;  /* bit left in the prev_word */
    int incnt_next;
    int bitcnt;
    void *userData;
} AVCDecBitstream;

typedef struct tagDecObject
{

    AVCCommonObj *common;

    AVCDecBitstream     *bitstream; /* for current NAL */

    /* sequence parameter set */
    AVCSeqParamSet *seqParams[32]; /* Array of pointers, get allocated at arrival of new seq_id */

    /* picture parameter set */
    AVCPicParamSet *picParams[256]; /* Array of pointers to picture param set structures */

    /* For internal operation, scratch memory for MV, prediction, transform, etc.*/
    uint    ref_idx_l0[4]; /* [mbPartIdx], te(v) */
    uint    ref_idx_l1[4];

    /* function pointers */
    AVCDec_Status(*residual_block)(struct tagDecObject*, int,  int,
                                   int *, int *, int *);
    /* Application control data */
    AVCHandle *avcHandle;
    void (*AVC_DebugLog)(AVCLogType type, char *string1, char *string2);
    /*bool*/
    uint    debugEnable;

} AVCDecObject;

#endif /* _AVCDEC_INT_H_ */
