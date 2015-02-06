
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                      MPEG-4 VideoMediaHeaderAtom Class                        */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef VIDEOMEDIAHEADERATOM_H_INCLUDED
#define VIDEOMEDIAHEADERATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef MEDIAINFORMATIONHEADERATOM_H_INCLUDED
#include "mediainformationheaderatom.h"
#endif

class VideoMediaHeaderAtom : public MediaInformationHeaderAtom
{

    public:
        VideoMediaHeaderAtom(MP4_FF_FILE *fp, uint32 size, uint32 type);
        virtual ~VideoMediaHeaderAtom();

        virtual uint32 getMediaInformationHeaderType() const;

    private:
        // Static reserved constants
        uint64 _reserved; // 8 (64bits)
};

#endif // VIDEOMEDIAHEADERATOM_H_INCLUDED


