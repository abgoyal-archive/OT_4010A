

#ifndef OSCLCONFIG_CHECK_H_INCLUDED
#define OSCLCONFIG_CHECK_H_INCLUDED


typedef int8 __int8__check__;
typedef uint8 __uint8__check__;
typedef int16 __int16__check__;
typedef uint16 __uint16__check__;
typedef int32 __int32__check__;
typedef uint32 __uint32__check__;

#ifndef OSCL_ASSERT_ALWAYS
#error "ERROR: OSCL_ASSERT_ALWAYS has to be defined to either 1 or 0."
#endif


#ifndef OSCL_DISABLE_INLINES
#error "ERROR: OSCL_DISABLE_INLINES has to be defined to either 1 or 0."
#endif

#ifndef OSCL_HAS_ANSI_STDLIB_SUPPORT
#error "ERROR: OSCL_HAS_ANSI_STDLIB_SUPPORT has to be defined to either 1 or 0."
#endif

#ifndef OSCL_HAS_ANSI_STDIO_SUPPORT
#error "ERROR: OSCL_HAS_ANSI_STDIO_SUPPORT has to be defined to either 1 or 0."
#endif

#ifndef OSCL_HAS_ANSI_STRING_SUPPORT
#error "ERROR: OSCL_HAS_ANSI_STRING_SUPPORT has to be defined to either 1 or 0."
#endif

#ifndef OSCL_HAS_UNICODE_SUPPORT
#error "ERROR: OSCL_HAS_UNICODE_SUPPORT has to be defined to either 1 or 0."
#endif

#ifndef _STRLIT
#error "ERROR: _STRLIT has to be defined."
#endif

#ifndef _STRLIT_CHAR
#error "ERROR: _STRLIT_CHAR has to be defined."
#endif

#if (OSCL_HAS_UNICODE_SUPPORT) && !defined(_STRLIT_WCHAR)
#error "ERROR: _STRLIT_WCHAR has to be defined"
#endif

#if (OSCL_HAS_UNICODE_SUPPORT) && !defined(OSCL_NATIVE_WCHAR_TYPE)
#error "ERROR: OSCL_NATIVE_WCHAR_TYPE has to be defined."
#endif

#ifndef OSCL_HAS_MSWIN_SUPPORT
#error "ERROR: OSCL_HAS_MSWIN_SUPPORT has to be defined to either 1 or 0"
#endif

#ifndef OSCL_HAS_UNIX_SUPPORT
#error "ERROR: OSCL_HAS_UNIX_SUPPORT has to be defined to either 1 or 0."
#endif

#ifndef OSCL_HAS_SYMBIAN_SUPPORT
#error "ERROR: OSCL_HAS_SYMBIAN_SUPPORT has to be defined to either 1 or 0"
#endif

#ifndef OSCL_INTEGERS_WORD_ALIGNED
#error "ERROR: OSCL_INTEGERS_WORD_ALIGNED has to be defined to either 1 or 0."
#endif

#ifndef OSCL_BYTE_ORDER_BIG_ENDIAN
#error "ERROR: OSCL_BYTE_ORDER_BIG_ENDIAN has to be defined to either 1 or 0."
#endif

#ifndef OSCL_BYTE_ORDER_LITTLE_ENDIAN
#error "ERROR: OSCL_BYTE_ORDER_LITTLE_ENDIAN has to be defined to either 1 or 0."
#endif

#if !(OSCL_BYTE_ORDER_BIG_ENDIAN) && !(OSCL_BYTE_ORDER_LITTLE_ENDIAN)
#error "ERROR: either OSCL_BYTE_ORDER_LITTLE_ENDIAN or else OSCL_BYTE_ORDER_BIG_ENDIAN must be 1."
#endif
#if (OSCL_BYTE_ORDER_BIG_ENDIAN) && (OSCL_BYTE_ORDER_LITTLE_ENDIAN)
#error "ERROR: either OSCL_BYTE_ORDER_LITTLE_ENDIAN or else OSCL_BYTE_ORDER_BIG_ENDIAN must be 1."
#endif

#ifndef OSCL_HAS_GLOBAL_VARIABLE_SUPPORT
#error "ERROR: OSCL_HAS_GLOBAL_VARIABLE_SUPPORT has to be defined to either 1 or 0."
#endif

#if !(OSCL_HAS_GLOBAL_VARIABLE_SUPPORT)
#ifndef OSCL_HAS_PARTIAL_GLOBAL_VARIABLE_SUPPORT
#error "ERROR: OSCL_HAS_PARTIAL_GLOBAL_VARIABLE_SUPPORT has to be defined to either 1 or 0."
#endif
#endif

#if (OSCL_BYTE_ORDER_LITTLE_ENDIAN) && (OSCL_BYTE_ORDER_BIG_ENDIAN)
#error "ERROR: Multiple selection for OSCL_BYTE_ORDER."
#endif

#ifndef OSCL_HAS_ANSI_STRING_SUPPORT
#error "ERROR: OSCL_HAS_ANSI_STRING_SUPPORT has to be defined to either 1 or 0."
#endif

#ifndef OSCL_HAS_NATIVE_INT64_TYPE
#error "ERROR: OSCL_HAS_NATIVE_INT64_TYPE has to be defined to either 1 or 0."
#endif

#ifndef OSCL_HAS_NATIVE_UINT64_TYPE
#error "ERROR: OSCL_HAS_NATIVE_UINT64_TYPE has to be defined to either 1 or 0."
#endif

#if OSCL_HAS_NATIVE_INT64_TYPE
#ifndef OSCL_NATIVE_INT64_TYPE
#error "ERROR: OSCL_NATIVE_INT64_TYPE has to be defined."
#endif
#endif

#if OSCL_HAS_NATIVE_UINT64_TYPE
#ifndef OSCL_NATIVE_UINT64_TYPE
#error "ERROR: OSCL_NATIVE_UINT64_TYPE has to be defined."
#endif
#endif

#if OSCL_HAS_NATIVE_INT64_TYPE
#ifndef INT64
#error "ERROR: INT64(x) has to be defined."
#endif
#endif

#if OSCL_HAS_NATIVE_UINT64_TYPE
#ifndef UINT64
#error "ERROR: UINT64(x) has to be defined."
#endif
#endif

#if OSCL_HAS_NATIVE_INT64_TYPE
#ifndef INT64_HILO
#error "ERROR: INT64_HILO(high,low) has to be defined."
#endif
#endif

#if OSCL_HAS_NATIVE_UINT64_TYPE
#ifndef UINT64_HILO
#error "ERROR: UINT64_HILO(high,low) has to be defined."
#endif
#endif

#ifndef OSCL_MEMFRAG_PTR_BEFORE_LEN
#error "ERROR: OSCL_MEMFRAG_PTR_BEFORE_LEN has to be defined to either 0 or 1"
#endif

#ifndef OSCL_HAS_TLS_SUPPORT
#error "ERROR: OSCL_HAS_TLS_SUPPORT has to be defined to either 1 or 0"
#endif

#ifndef OSCL_TLS_IS_KEYED
#error "ERROR: OSCL_TLS_IS_KEYED has to be defined to either 1 or 0"
#endif


#if (OSCL_TLS_IS_KEYED) && !defined(OSCL_TLS_STORE_FUNC)
#error "ERROR: OSCL_TLS_STORE_FUNC has to be defined"
#endif

#if (OSCL_TLS_IS_KEYED) && !defined(OSCL_TLS_GET_FUNC)
#error "ERROR: OSCL_TLS_GET_FUNC has to be defined"
#endif

#if (OSCL_TLS_IS_KEYED) && !defined(OSCL_TLS_KEY_CREATE_FUNC)
#error "ERROR: OSCL_TLS_KEY_CREATE_FUNC has to be defined"
#endif

#if (OSCL_TLS_IS_KEYED) && !defined(OSCL_TLS_KEY_DELETE_FUNC)
#error "ERROR: OSCL_TLS_KEY_DELETE_FUNC has to be defined"
#endif


#if (OSCL_HAS_TLS_SUPPORT) && !(OSCL_TLS_IS_KEYED) && !defined(OSCL_TLS_STORE_FUNC)
#error "ERROR: OSCL_TLS_STORE_FUNC has to be defined"
#endif

#if (OSCL_HAS_TLS_SUPPORT) && !(OSCL_TLS_IS_KEYED) && !defined(OSCL_TLS_GET_FUNC)
#error "ERROR: OSCL_TLS_GET_FUNC has to be defined"
#endif

#if !defined(OSCL_HAS_BASIC_LOCK )
#error "ERROR: OSCL_HAS_BASIC_LOCK must be defined to 0 or 1"
#endif


#if (OSCL_HAS_BASIC_LOCK) && !defined(TOsclBasicLockObject)
typedef TOsclBasicLockObject __verify__TOsclBasicLockObject__defined__;
#endif

/*! @} */

#endif // OSCLCONFIG_CHECK_H_INCLUDED


