

#ifndef PV_AMR_WB_TYPE_DEFS_H
#define PV_AMR_WB_TYPE_DEFS_H

#include    "oscl_base.h"


#ifndef Word8
typedef int8        Word8;
#endif

#ifndef UWord8
typedef uint8       UWord8;
#endif

#ifndef Int
typedef signed int  Int;
#endif

#ifndef UInt
typedef unsigned int    UInt;
#endif



#ifndef INT16_MIN
#define INT16_MIN   (-32768)
#endif

#ifndef INT16_MAX
#define INT16_MAX   32767
#endif




#ifndef INT32_MIN
#define INT32_MIN   (-2147483647 - 1)
#endif
#ifndef INT32_MAX
#define INT32_MAX   2147483647
#endif


#ifndef UINT32_MIN
#define UINT32_MIN  0
#endif
#ifndef UINT32_MAX
#define UINT32_MAX  0xffffffff
#endif


#ifndef INT_MAX
#define INT_MAX  INT32_MAX      /*  for 32 bit  */
#endif




#ifndef Flag
typedef Int Flag;
#endif

#ifndef Bool
typedef Int     Bool;
#endif
#ifndef FALSE
#define FALSE       0
#endif

#ifndef TRUE
#define TRUE        1
#endif

#ifndef OFF
#define OFF     0
#endif
#ifndef ON
#define ON      1
#endif

#ifndef NO
#define NO      0
#endif
#ifndef YES
#define YES     1
#endif

#ifndef SUCCESS
#define SUCCESS     0
#endif

#ifndef  NULL
#define  NULL       0
#endif


#endif  /* PV_AMR_WB_TYPE_DEFS_H */
