
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                      MPEG-4 SoundMediaHeaderAtom Class                        */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef SOUNDMEDIAHEADERATOM_H_INCLUDED
#define SOUNDMEDIAHEADERATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef MEDIAINFORMATIONHEADERATOM_H_INCLUDED
#include "mediainformationheaderatom.h"
#endif

class SoundMediaHeaderAtom : public MediaInformationHeaderAtom
{

    public:
        SoundMediaHeaderAtom(MP4_FF_FILE *fp, uint32 size, uint32 type);
        virtual ~SoundMediaHeaderAtom();

        virtual uint32 getMediaInformationHeaderType() const;

    private:
        // Static reserved constants
        uint32 _reserved;
};

#endif // SOUNDMEDIAHEADERATOM_H_INCLUDED


