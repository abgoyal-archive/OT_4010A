
#ifndef PVMF_FILEFORMAT_EVENTS_H_INCLUDED
#define PVMF_FILEFORMAT_EVENTS_H_INCLUDED

#define PVMFFileFormatEventTypesUUID PVUuid(0x1ccf06b0,0xb6ea,0x4c29,0x9d,0xbe,0xca,0x0a,0x8c,0xc5,0x80,0x1a)

typedef enum
{
    /**
     File open error
    **/
    PVMFFFErrFileOpen = 1024,

    /**
     File read error
    **/
    PVMFFFErrFileRead,

    /**
     File seek error
    **/
    PVMFFFErrFileSeek,

    /**
     Parsed data is invalid
    **/
    PVMFFFErrInvalidData,

    /**
     Insufficient buffer to read in the data
    **/
    PVMFFFErrInsufficientBuffer,

    /**
     Insufficient data available for the specified type
    **/
    PVMFFFErrInsufficientData,

    /**
     Specified type of data is not supported
    **/
    PVMFFFErrNotSupported,

    /**
     Unknown data type
    **/
    PVMFFFErrUnknownData,

    /**
     Miscellaneous error
    **/
    PVMFFFErrMisc,

    /**
     Placeholder for the last PVMF FF error event
     **/
    PVMFFFErrLast = 8191
} PVMFFileFormatErrorEventType;

typedef enum
{
    /**
     End of file detected
     **/
    PVMFFFInfoEndOfFile = 8192,

    /**
     Not supported so skipped
     **/
    PVMFFFInfoNotSupported,

    /**
     Placeholder for the last PVMF FF informational event
     **/
    PVMFFFInfoLast = 10000

} PVMFFileFormatInformationalEventType;

#endif // PVMF_FILEFORMAT_EVENTS_H_INCLUDED


