
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                          MPEG-4 Util: H263DecoderSpecificInfo                 */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/

#ifndef H263DECODERSPECIFICINFO_H_INCLUDED
#define H263DECODERSPECIFICINFO_H_INCLUDED

#ifndef ATOMDEFS_H_INCLUDED
#include "atomdefs.h"
#endif

#ifndef DECODERSPECIFICINFO_H_INCLUDED
#include "decoderspecificinfo.h"
#endif


class H263DecoderSpecificInfo : public DecoderSpecificInfo
{

    public:
        H263DecoderSpecificInfo(MP4_FF_FILE *fp, bool o3GPPTrack = false); // Default constructor
        virtual ~H263DecoderSpecificInfo() {}; // Destructor

        uint32 getVendorCode()
        {
            return _VendorCode;
        }
        int8  getEncoderVersion()
        {
            return _codec_version;
        }
        int8  getCodecProfile()
        {
            return _codec_profile;
        }
        int8  getCodecLevel()
        {
            return _codec_level;
        }
        int16 getMaxWidth()
        {
            return _max_width;
        }
        int16 getMaxHeight()
        {
            return _max_height;
        }


        uint32       _VendorCode;
        uint8        _codec_version;
        uint8        _codec_profile;
        uint8        _codec_level;
        uint16       _max_width;
        uint16       _max_height;
};

#endif  // H263DECODERSPECIFICINFO_H_INCLUDED


