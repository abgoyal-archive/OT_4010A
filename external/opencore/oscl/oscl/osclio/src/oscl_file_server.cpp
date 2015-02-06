

#include "oscl_file_server.h"
#include "oscl_file_types.h"
#include "oscl_stdstring.h"
#include "oscl_utf8conv.h"


OSCL_EXPORT_REF Oscl_FileServer::Oscl_FileServer()
{
}

OSCL_EXPORT_REF Oscl_FileServer::~Oscl_FileServer()
{
}

OSCL_EXPORT_REF int32 Oscl_FileServer::Connect()
{
    //default-- assume there is nothing needed.
    return 0;
}

OSCL_EXPORT_REF int32 Oscl_FileServer::Close()
{
    //default-- assume there is nothing needed.
    return 0;
}

OSCL_EXPORT_REF int32 Oscl_FileServer::Oscl_DeleteFile(const oscl_wchar* filename)
{
    {
        //Convert to UTF8
        char convfilename[OSCL_IO_FILENAME_MAXLEN];
        if (0 == oscl_UnicodeToUTF8(filename, oscl_strlen(filename), convfilename, OSCL_IO_FILENAME_MAXLEN))
            return (-1);

        return unlink(convfilename);

    }
}

OSCL_EXPORT_REF int32 Oscl_FileServer::Oscl_DeleteFile(const char* filename)
{
    {
        return unlink(filename);
    }
}



