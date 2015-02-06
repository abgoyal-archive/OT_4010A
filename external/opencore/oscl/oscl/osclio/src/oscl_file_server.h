
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                     O S C L _ F I L E  _ SERVER

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_FILE_SERVER_H_INCLUDED
#define OSCL_FILE_SERVER_H_INCLUDED

#ifndef OSCLCONFIG_IO_H_INCLUDED
#include "osclconfig_io.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

class Oscl_File;

class Oscl_FileServer
{
    public:
        /**
         * Constructor
         */
        OSCL_IMPORT_REF Oscl_FileServer();

        /**
         * Destructor
         */
        OSCL_IMPORT_REF ~Oscl_FileServer();

        /**
         * Connects the server.  This must be called before a file
         * server can be used.
         *
         * @return returns 0 on success and a non-zero value otherwise
         */
        OSCL_IMPORT_REF int32 Connect();

        /**
         * Closes a file server.
         *
         * @return returns 0 on success and a non-zero value otherwise
         */
        OSCL_IMPORT_REF int32 Close();

        /**
         * Deletes a file from the filesystem
         * * @param filename name of the file to delete (Utf8)
         *
         * @return returns 0 if successful, and a non-zero value otherwise.
         */
        OSCL_IMPORT_REF int32 Oscl_DeleteFile(const char *filename);

        /**
         * Deletes a file from the filesystem
         *
         * @param filename name of the file to delete (Unicode)
         *
         * @return returns 0 if successful, and a non-zero value otherwise.
         */
        OSCL_IMPORT_REF int32 Oscl_DeleteFile(const oscl_wchar *filename);


    protected:
        friend class Oscl_File;
        friend class OsclNativeFile;

        //file server objects (if appropriate)
};



#endif // OSCL_FILE_SERVER_H_INCLUDED

/*! @} */

