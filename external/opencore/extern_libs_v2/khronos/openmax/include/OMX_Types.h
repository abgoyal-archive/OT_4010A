


#ifndef OMX_Types_h
#define OMX_Types_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef __SYMBIAN32__   
#   ifdef __OMX_EXPORTS
#       define OMX_API __declspec(dllexport)
#   else
#       ifdef _WIN32
#           define OMX_API __declspec(dllexport) 
#       else
#           define OMX_API __declspec(dllimport)
#       endif
#   endif
#else
#   ifdef _WIN32
#      ifdef __OMX_EXPORTS
#          define OMX_API __declspec(dllexport)
#      else
//#          define OMX_API __declspec(dllimport)
#define OMX_API
#      endif
#   else
#      ifdef __OMX_EXPORTS
#          define OMX_API
#      else
#          define OMX_API extern
#      endif
#   endif
#endif

#ifndef OMX_APIENTRY
#define OMX_APIENTRY 
#endif 

#ifndef OMX_IN
#define OMX_IN
#endif

#ifndef OMX_OUT
#define OMX_OUT
#endif


#ifndef OMX_INOUT
#define OMX_INOUT
#endif

#define OMX_ALL 0xFFFFFFFF

/** In the following we define groups that help building doxygen documentation */

 
 /** @defgroup comp OpenMAX IL component
 * Functions and structure related to the OMX IL component
 */
 

  
 
 
 /** @defgroup metadata Metadata handling
  * 
  */ 

/** OMX_U8 is an 8 bit unsigned quantity that is byte aligned */
typedef unsigned char OMX_U8;

/** OMX_S8 is an 8 bit signed quantity that is byte aligned */
typedef signed char OMX_S8;

/** OMX_U16 is a 16 bit unsigned quantity that is 16 bit word aligned */
typedef unsigned short OMX_U16;

/** OMX_S16 is a 16 bit signed quantity that is 16 bit word aligned */
typedef signed short OMX_S16;

/** OMX_U32 is a 32 bit unsigned quantity that is 32 bit word aligned */
typedef unsigned long OMX_U32;

/** OMX_S32 is a 32 bit signed quantity that is 32 bit word aligned */
typedef signed long OMX_S32;


#ifndef OMX_SKIP64BIT
#ifdef __SYMBIAN32__
/** OMX_U64 is a 64 bit unsigned quantity that is 64 bit word aligned */
typedef unsigned long long OMX_U64;

/** OMX_S64 is a 64 bit signed quantity that is 64 bit word aligned */
typedef signed long long OMX_S64;

#elif defined(WIN32)

/** OMX_U64 is a 64 bit unsigned quantity that is 64 bit word aligned */   
typedef unsigned __int64  OMX_U64;

/** OMX_S64 is a 64 bit signed quantity that is 64 bit word aligned */
typedef signed   __int64  OMX_S64;

#else /* WIN32 */

/** OMX_U64 is a 64 bit unsigned quantity that is 64 bit word aligned */
typedef unsigned long long OMX_U64;

/** OMX_S64 is a 64 bit signed quantity that is 64 bit word aligned */
typedef signed long long OMX_S64;

#endif /* WIN32 */
#endif


typedef enum OMX_BOOL {
    OMX_FALSE = 0,
    OMX_TRUE = !OMX_FALSE,
    OMX_BOOL_MAX = 0x7FFFFFFF
} OMX_BOOL; 
 
typedef void* OMX_PTR;

typedef char* OMX_STRING;

typedef unsigned char* OMX_BYTE;

typedef unsigned char OMX_UUIDTYPE[128];

typedef enum OMX_DIRTYPE
{
    OMX_DirInput,              /**< Port is an input port */
    OMX_DirOutput,             /**< Port is an output port */
    OMX_DirMax = 0x7FFFFFFF
} OMX_DIRTYPE;

typedef enum OMX_ENDIANTYPE
{
    OMX_EndianBig, /**< big endian */
    OMX_EndianLittle, /**< little endian */
    OMX_EndianMax = 0x7FFFFFFF
} OMX_ENDIANTYPE;


typedef enum OMX_NUMERICALDATATYPE
{
    OMX_NumericalDataSigned, /**< signed data */
    OMX_NumericalDataUnsigned, /**< unsigned data */
    OMX_NumercialDataMax = 0x7FFFFFFF
} OMX_NUMERICALDATATYPE;


/** Unsigned bounded value type */
typedef struct OMX_BU32 {
    OMX_U32 nValue; /**< actual value */
    OMX_U32 nMin;   /**< minimum for value (i.e. nValue >= nMin) */
    OMX_U32 nMax;   /**< maximum for value (i.e. nValue <= nMax) */
} OMX_BU32;


/** Signed bounded value type */
typedef struct OMX_BS32 {
    OMX_S32 nValue; /**< actual value */
    OMX_S32 nMin;   /**< minimum for value (i.e. nValue >= nMin) */
    OMX_S32 nMax;   /**< maximum for value (i.e. nValue <= nMax) */
} OMX_BS32;


#ifndef OMX_SKIP64BIT
typedef OMX_S64 OMX_TICKS;
#else
typedef struct OMX_TICKS
{
    OMX_U32 nLowPart;    /** low bits of the signed 64 bit tick value */
    OMX_U32 nHighPart;   /** high bits of the signed 64 bit tick value */
} OMX_TICKS;
#endif
#define OMX_TICKS_PER_SECOND 1000000

typedef void* OMX_HANDLETYPE;

typedef struct OMX_MARKTYPE
{
    OMX_HANDLETYPE hMarkTargetComponent;   /**< The component that will 
                                                generate a mark event upon 
                                                processing the mark. */
    OMX_PTR pMarkData;   /**< Application specific data associated with 
                              the mark sent on a mark event to disambiguate 
                              this mark from others. */
} OMX_MARKTYPE;


typedef void* OMX_NATIVE_DEVICETYPE;

typedef void* OMX_NATIVE_WINDOWTYPE;

typedef union OMX_VERSIONTYPE
{
    struct
    {
        OMX_U8 nVersionMajor;   /**< Major version accessor element */
        OMX_U8 nVersionMinor;   /**< Minor version accessor element */
        OMX_U8 nRevision;       /**< Revision version accessor element */
        OMX_U8 nStep;           /**< Step version accessor element */
    } s;
    OMX_U32 nVersion;           /**< 32 bit value to make accessing the
                                    version easily done in a single word
                                    size copy/compare operation */
} OMX_VERSIONTYPE;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/* File EOF */
