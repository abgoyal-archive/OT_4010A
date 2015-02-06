
#ifndef     __SBC_TYPE_DEFS__
#define     __SBC_TYPE_DEFS__

#ifdef __cplusplus
extern "C"
{
#endif


    /*********************************************
     * Include files
     *********************************************/
#include    "oscl_types.h"

    /*********************************************
     * Defines
     *********************************************/

    /*=============================================================================*
     *==== TRUE / FALSE ===========================================================*
     *=============================================================================*/
#ifndef FALSE
#   define                  FALSE                           0
#endif

#ifndef TRUE
#   define                  TRUE                            1
#endif

    /*=============================================================================*
     *==== TRUE / FALSE ===========================================================*
     *=============================================================================*/
//#undef NULL
#ifndef NULL
#ifdef  __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif
    /*=============================================================================*
     *==== EXIT_SUCCESS / EXIT_FAILURE=============================================*
     *=============================================================================*/
#ifndef EXIT_SUCCESS
#   define                  EXIT_SUCCESS                    0
#endif

#ifndef EXIT_FAILURE
#   define                  EXIT_FAILURE                    1
#endif

    /*----------------------------------------------------*
     * define char type
     *----------------------------------------------------*/
    typedef char Char;

    /*----------------------------------------------------*
     * define 8 bit signed/unsigned types & constants
     *----------------------------------------------------*/
#ifndef Word8
    typedef                 int8                     Word8;
#endif

#ifndef UWord8
    typedef                 uint8                   UWord8;
#endif

    /*----------------------------------------------------*
     * define 16 bit signed/unsigned types & constants
     *----------------------------------------------------*/
#ifndef Word16
    typedef                 int16                             Word16;
#endif

#ifndef UWord16
    typedef                 uint16                    UWord16;
#endif

    /*----------------------------------------------------*
     * define 32 bit signed/unsigned types & constants
     *----------------------------------------------------*/
#ifndef Word32
    typedef                 int32                             Word32;
#endif

#ifndef UWord32
    typedef                 uint32                    UWord32;
#endif

    /*----------------------------------------------------*
     * targate platform integer
     *----------------------------------------------------*/
    typedef                    int                              Int;
    typedef                    unsigned int                     UInt;
//typedef                    __int64                          Int64;
    typedef                    int64                 Int64;
    typedef                   uint64                 UInt64;

    /*----------------------------------------------------*
     * define boolean type
     *----------------------------------------------------*/
    typedef enum { Bfalse, Btrue } Boolean;

#ifdef __cplusplus
}
#endif

#endif      /* __SYSTEM__ */




