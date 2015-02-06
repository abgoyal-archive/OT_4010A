
#ifndef PV_AVIFILE_H_INCLUDED
#include "pv_avifile.h"
#endif

#ifndef PV_AVIFILE_PARSER_H_INCLUDED
#include "pv_avifile_parser.h"
#endif

#ifndef OSCL_DLL_H_INCLUDED
#include "oscl_dll.h"
#endif

OSCL_DLL_ENTRY_POINT_DEFAULT()

OSCL_EXPORT_REF PVAviFile* PVAviFile::CreateAviFileParser(OSCL_wHeapString<OsclMemAllocator> aFileName, int32& aError, Oscl_FileServer* aFileServer,
        PVMFCPMPluginAccessInterfaceFactory*  aCPM, OsclFileHandle*  aFileHandle)
{

    PVAviFileParser* pAviFileParser = NULL;

    OSCL_TRY(aError, pAviFileParser =
                 OSCL_NEW(PVAviFileParser, (aFileName, aError, aFileServer, aCPM, aFileHandle));
            );

    if (PV_AVI_FILE_PARSER_SUCCESS == aError)
    {
        pAviFileParser->ParseFile();
        aError = pAviFileParser->GetStatus(); // get & log error, if any

        if (PV_AVI_FILE_PARSER_SUCCESS == aError)
        {
            return pAviFileParser;
        }
        else
        {
            OSCL_DELETE(pAviFileParser);
            pAviFileParser = NULL;
            return NULL;
        }
    }
    else if (pAviFileParser != NULL)
    {
        OSCL_DELETE(pAviFileParser);
        pAviFileParser = NULL;
        return NULL;
    }

    return NULL;
}


OSCL_EXPORT_REF void PVAviFile::DeleteAviFileParser(PVAviFile* apAviFileParser)
{
    if (NULL != apAviFileParser)
    {
        PVAviFileParser* ptr = OSCL_STATIC_CAST(PVAviFileParser*, apAviFileParser);
        OSCL_DELETE(ptr);
        ptr = NULL;
        apAviFileParser = NULL;
    }

}


