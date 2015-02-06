
#ifndef MP4FFFREC_H_INCLUDED
#define MP4FFFREC_H_INCLUDED

#ifndef OSCL_FILE_SERVER_H_INCLUDED
#include "oscl_file_server.h"
#endif

#ifndef OSCL_FILE_HANDLE_H_INCLUDED
#include "oscl_file_handle.h"
#endif

/* CPM Related Header Files */
#ifndef CPM_H_INCLUDED
#include "cpm.h"
#endif

class MP4FileRecognizer
{

    public:

        OSCL_IMPORT_REF static bool IsMP4File(OSCL_wString& filename,
                                              Oscl_FileServer* fileServSession = NULL);

        OSCL_IMPORT_REF static bool IsMP4File(Oscl_File* filePtr);

        OSCL_IMPORT_REF static bool IsMP4File(PVMFCPMPluginAccessInterfaceFactory* aCPMAccessFactory,
                                              Oscl_FileServer* aFileServSession = NULL,
                                              OsclFileHandle* aHandle = NULL);

};

#endif
