
#ifndef PVMF_MP3FFPARSER_NODE_H_INCLUDED
#include "pvmf_mp3ffparser_node.h"
#endif
#ifndef PVMF_MP3FFPARSER_FACTORY_H_INCLUDED
#include "pvmf_mp3ffparser_factory.h"
#endif
#ifndef OSCL_ERROR_CODES_H_INCLUDED
#include "oscl_error_codes.h"
#endif
#ifndef OSCL_EXCEPTION_H_INCLUDED
#include "oscl_exception.h"
#endif
// Use default DLL entry point for Symbian
#ifndef OSCL_DLL_H_INCLUDED
#include "oscl_dll.h"
#endif

OSCL_DLL_ENTRY_POINT_DEFAULT()


////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF
PVMFNodeInterface* PVMFMP3FFParserNodeFactory::CreatePVMFMP3FFParserNode(int32 aPriority)
{
    PVMFNodeInterface* node = NULL;
    node = OSCL_NEW(PVMFMP3FFParserNode, (aPriority));
    if (node == NULL)
    {
        OSCL_LEAVE(OsclErrNoMemory);
    }
    return node;
}

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF
bool PVMFMP3FFParserNodeFactory::DeletePVMFMP3FFParserNode(PVMFNodeInterface* aNode)
{
    if (aNode)
    {
        OSCL_DELETE(aNode);
        return true;
    }
    return false;
}


