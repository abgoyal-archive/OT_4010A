
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                     O S C L _ F I L E  _ N A T I V E

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_FILE_NATIVE_H_INCLUDED
#define OSCL_FILE_NATIVE_H_INCLUDED

#ifndef OSCLCONFIG_IO_H_INCLUDED
#include "osclconfig_io.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_AOSTATUS_H_INCLUDED
#include "oscl_aostatus.h"
#endif

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef OSCL_FILE_TYPES_H_INCLUDED
#include "oscl_file_types.h"
#endif



class Oscl_FileServer;
class OsclNativeFile : public HeapBase
{
    public:
        OsclNativeFile();
        ~OsclNativeFile();

        // These are the Native OS File IO calls.
        int32  Open(const OsclFileHandle&, uint32 mode
                    , const OsclNativeFileParams& params
                    , Oscl_FileServer& fileserv);
        int32  Open(const oscl_wchar *filename, uint32 mode
                    , const OsclNativeFileParams& params
                    , Oscl_FileServer& fileserv);
        int32  Open(const char *filename, uint32 mode
                    , const OsclNativeFileParams& params
                    , Oscl_FileServer& fileserv);
        uint32 Read(OsclAny *buffer, uint32 size, uint32 numelements);
        uint32 Write(const OsclAny *buffer, uint32 size, uint32 numelements);
        int32  Seek(TOsclFileOffset offset, Oscl_File::seek_type origin);
        TOsclFileOffset  Tell();
        int32  Flush();
        int32  EndOfFile();
        TOsclFileOffset Size();
        int32 Close();

        //query for current open mode.
        uint32 Mode()
        {
            return iMode;
        }

        //get last filesystem error.
        int32 GetError();

        /*!
        ** Asynchronous read.
        **
        ** @param buffer: data buffer, must be at least size*numelements bytes
        ** @param size: size of elements
        ** @param numelements: number of elements to read
        ** @param status: Request status for asynchronous completion
        ** @returns: 0 for success.
        **/
        int32 ReadAsync(OsclAny*buffer, uint32 size, uint32 numelements, OsclAOStatus& status);
        /*!
        ** Get the number of elements read in the last call to ReadAsync.
        ** @returns: number of elements read.
        **/
        uint32 GetReadAsyncNumElements();
        /*!
        ** @returns: true if async read is supported natively.
        **/
        bool HasAsyncRead();
        /*!
        ** Cancel any pending async read.
        **/
        void ReadAsyncCancel();

    private:
        int32 OpenFileOrSharedFd(const char *filename, const char *openmode);

        //current open mode
        uint32 iMode;

        //was an open file handle passed in?
        bool iOpenFileHandle;

        //native file object.
        FILE* iFile;

        bool iWriteDirty;

#ifdef ENABLE_SHAREDFD_PLAYBACK
        int  iSharedFd;
        long long iSharedFileOffset;
        long long iSharedFileSize;
        long long iSharedFilePosition;
#endif
};

#endif // OSCL_FILE_NATIVE_H_INCLUDED

/*! @} */

