
/*********************************************************************************/
/* Revision History                                                             */
/* Date: 11/04/05                                                               */
/* Description: Created for abstracting out OSCL such that the code can be used */
/*          by both V3 and V4 OSCL library. This file is for V4.                */
/*********************************************************************************/

#ifndef _M4VENC_OSCL_H_
#define _M4VENC_OSCL_H_


#define OSCL_DISABLE_WARNING_CONV_POSSIBLE_LOSS_OF_DATA
#include "osclconfig_compiler_warnings.h"

#include "oscl_mem.h"

#define M4VENC_MALLOC(size)   oscl_malloc(size)
#define M4VENC_FREE(ptr)                oscl_free(ptr)

#define M4VENC_MEMSET(ptr,val,size)     oscl_memset(ptr,val,size)
#define M4VENC_MEMCPY(dst,src,size)     oscl_memcpy(dst,src,size)

#include "oscl_math.h"
#define M4VENC_LOG(x)                   oscl_log(x)
#define M4VENC_SQRT(x)                  oscl_sqrt(x)
#define M4VENC_POW(x,y)                 oscl_pow(x,y)

#define M4VENC_HAS_SYMBIAN_SUPPORT  OSCL_HAS_SYMBIAN_SUPPORT

#endif //_M4VENC_OSCL_H_
