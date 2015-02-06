
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                     O S C L _ F I L E  _ TYPES

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_FILE_TYPES_H_INCLUDED
#define OSCL_FILE_TYPES_H_INCLUDED


#define OSCL_IO_FILENAME_MAXLEN 512
#define OSCL_IO_EXTENSION_MAXLEN 512

#define OSCL_FILE_WCHAR_PATH_DELIMITER _STRLIT("/")
#define OSCL_FILE_CHAR_PATH_DELIMITER _STRLIT_CHAR("/")

//a class for passing configuration options from Oscl_File to OsclNativeFile
class OsclNativeFileParams
{
    public:
        OsclNativeFileParams(uint32 mode = 0, uint32 bufsize = 0, uint32 asyncsize = 0)
                : iNativeAccessMode(mode)
                , iNativeBufferSize(bufsize)
                , iAsyncReadBufferSize(asyncsize)
        {}
        uint32 iNativeAccessMode;
        uint32 iNativeBufferSize;
        uint32 iAsyncReadBufferSize;
};


#endif // OSCL_FILE_TYPES_H_INCLUDED

/*! @} */

