
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                     O S C L _ E R R O R _ C O D E S

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_ERROR_CODES_H_INCLUDED
#define OSCL_ERROR_CODES_H_INCLUDED


typedef int32 OsclLeaveCode;

#define OsclErrNone 0
#define OsclErrGeneral 100
#define OsclErrNoMemory 101
#define OsclErrCancelled 102
#define OsclErrNotSupported 103
#define OsclErrArgument 104
#define OsclErrBadHandle 105
#define OsclErrAlreadyExists 106
#define OsclErrBusy 107
#define OsclErrNotReady 108
#define OsclErrCorrupt 109
#define OsclErrTimeout 110
#define OsclErrOverflow 111
#define OsclErrUnderflow 112
#define OsclErrInvalidState 113
#define OsclErrNoResources 114
#define OsclErrNotInstalled 115
#define OsclErrAlreadyInstalled 116
#define OsclErrSystemCallFailed 117
#define OsclErrNoHandler 118
#define OsclErrThreadContextIncorrect 119

#define OSCL_ERR_NONE OsclErrNone
#define OSCL_BAD_ALLOC_EXCEPTION_CODE OsclErrNoMemory

typedef int32 OsclReturnCode;

#define  OsclSuccess 0
#define  OsclPending 1
#define  OsclFailure -1

#endif

/*! @} */
