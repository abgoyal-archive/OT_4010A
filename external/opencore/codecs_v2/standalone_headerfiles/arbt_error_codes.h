

#ifndef ARBT_ERROR_CODES_H_INCLUDED
#define ARBT_ERROR_CODES_H_INCLUDED


typedef int32 ArbtLeaveCode;

#define ArbtErrNone 0
#define ArbtErrGeneral 100
#define ArbtErrNoMemory 101
#define ArbtErrCancelled 102
#define ArbtErrNotSupported 103
#define ArbtErrArgument 104
#define ArbtErrBadHandle 105
#define ArbtErrAlreadyExists 106
#define ArbtErrBusy 107
#define ArbtErrNotReady 108
#define ArbtErrCorrupt 109
#define ArbtErrTimeout 110
#define ArbtErrOverflow 111
#define ArbtErrUnderflow 112
#define ArbtErrInvalidState 113
#define ArbtErrNoResources 114

#define ARBT_ERR_NONE ArbtErrNone
#define ARBT_BAD_ALLOC_EXCEPTION_CODE ArbtErrNoMemory

typedef int32 ArbtReturnCode;

#define  ArbtSuccess 0
#define  ArbtPending 1
#define  ArbtFailure -1

#endif

/*! @} */
