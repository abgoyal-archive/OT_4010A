
/*  =====================================================================   */
/*  File: pcma_mediaInfo.h                                                  */
/*  Description:                                                            */
/*                                                                          */
/*                                                                          */
/*  Rev:                                                                    */
/*  Created: 01/18/06                                                       */
/*  =====================================================================   */
/*                                                                          */
/*  Revision History:                                                       */
/*                                                                          */
/*  Rev:                                                                    */
/*  Date:                                                                   */
/*  Author:                                                                 */
/*  Description:                                                            */
/*                                                                          */
/* //////////////////////////////////////////////////////////////////////// */

#ifndef PCMA_MEDIAINFO_H
#define PCMA_MEDIAINFO_H

#include "sdp_memory.h"
#include "media_info.h"

class pcma_mediaInfo : public mediaInfo
{
    public:
        pcma_mediaInfo()
        {
            setMIMEType("PCMA");
        };

        ~pcma_mediaInfo() {};
};

#endif
