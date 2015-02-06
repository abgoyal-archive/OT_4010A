


#ifndef MP3DEC_EXP_H
#define MP3DEC_EXP_H
#ifdef __cplusplus
extern "C"{
#endif 

/*   ASSERT Definition */
#if defined(__MTK_TARGET__)
#include "kal_release.h"
#else
#include <assert.h>

#ifndef ASSERT
#define ASSERT(x)    assert(x)
#endif

#ifndef NULL
#define NULL    0
#endif
#endif

typedef struct {
	short sampleRateIndex;
	short CHNumber;	
	int PCMSamplesPerCH;
}Mp3dec_handle;


/*----------------------------------------------------------------------*/
/* Get required buffer size for MP3 Decoder                             */
/*----------------------------------------------------------------------*/
OSCL_IMPORT_REF void MP3Dec_GetMemSize(int *Min_BS_size,        /* Output, the required min. Bitsream buffer size in byte*/
							  int *PCM_size,           /* Output, the required PCM buffer size in byte          */
							  int *Working_BUF1_size,   /* Output, the required Working buffer1 size in byte    */
                              int *Working_BUF2_size);  /* Output, the required Working buffer2 size in byte    */


/*----------------------------------------------------------------------*/
/* Get the MP3 decoder handler.                                         */
/*   Return: the handle of current MP3 Decoder                          */
/*----------------------------------------------------------------------*/
OSCL_IMPORT_REF Mp3dec_handle* MP3Dec_Init(void *pWorking_BUF1,          /* Input, pointer to Working buffer1  */
						   void *pWorking_BUF2);         /* Input, pointer to Working buffer2  */

/*----------------------------------------------------------------------*/
/* Decompress the bitstream to PCM data                                 */
/*   Return: The consumed data size of Bitsream buffer for this  frame  */
/*----------------------------------------------------------------------*/
OSCL_IMPORT_REF int MP3Dec_Decode(Mp3dec_handle* handle,             /* the handle of current MP3 Decoder    */
                  void *pPCM_BUF,                    /* Input, pointer to PCM buffer         */
                  void *pBS_BUF,                     /* Input, pointer to Bitsream buffer    */
                  int BS_BUF_size,                   /* Input, the Bitsream buffer size      */
                  void *pBS_Read);                   /* Input, bitstream buffer read pointer */


#ifdef __cplusplus
}
#endif 

#endif /*MP3DEC_EXP_H*/
