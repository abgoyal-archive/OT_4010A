
#include "still_image_media_info_parser.h"
#include "oscl_string_utils.h"

SDP_ERROR_CODE
SDPStillImageMediaInfoParser::parseMediaInfo(const char *buff, const int index, SDPInfo *sdp, payloadVector payload_vec, bool isSipSdp, int alt_id, bool alt_def_id)
{

    OSCL_UNUSED_ARG(buff);
    OSCL_UNUSED_ARG(index);
    OSCL_UNUSED_ARG(payload_vec);
    OSCL_UNUSED_ARG(isSipSdp);
    OSCL_UNUSED_ARG(alt_id);
    OSCL_UNUSED_ARG(alt_def_id);

    void *memory = sdp->alloc(sizeof(still_image_mediaInfo), 0);
    if (NULL == memory)
    {
        PVMF_SDP_PARSER_LOGERROR((0, "SDPStillImageMediaInfoParser::parseMediaInfo - Unable to allocate memory"));
        return SDP_NO_MEMORY;
    }
    else
    {
        still_image_mediaInfo *still_imageI = OSCL_PLACEMENT_NEW(memory, still_image_mediaInfo());

        still_imageI->setMediaInfoID(sdp->getMediaObjectIndex());

        if (NULL == still_imageI)
            return SDP_FAILURE;
        return SDP_SUCCESS;
    }

}

