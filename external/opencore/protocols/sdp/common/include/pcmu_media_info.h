
/*  =====================================================================   */
/*  File: pcmu_mediaInfo.h                                                  */
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

#ifndef PCMU_MEDIAINFO_H
#define PCMU_MEDIAINFO_H

#include "sdp_memory.h"
#include "media_info.h"

class pcmu_mediaInfo : public mediaInfo
{
    public:
        pcmu_mediaInfo()
        {
            setMIMEType("PCMU");
        };

        ~pcmu_mediaInfo() {};
};

#endif
