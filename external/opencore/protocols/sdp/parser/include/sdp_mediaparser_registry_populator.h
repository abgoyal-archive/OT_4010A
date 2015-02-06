
#ifndef SDP_MEDIA_PARSER_REGISTRY_POPULATOR
#define SDP_MEDIA_PARSER_REGISTRY_POPULATOR
#include "sdp_mediaparser_registry.h"
class SDPMediaParserRegistryPopulater
{
    public:
        static OSCL_IMPORT_REF SDPMediaParserRegistry* PopulateRegistry();
        static OSCL_IMPORT_REF void CleanupRegistry(SDPMediaParserRegistry*& aMediaParserRegistry);
};
#endif
