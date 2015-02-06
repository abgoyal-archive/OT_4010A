
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                      MPEG-4 Mpeg4MediaHeaderAtom Class                        */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef MPEG4MEDIAHEADERATOM_H_INCLUDED
#define MPEG4MEDIAHEADERATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef MEDIAINFORMATIONHEADERATOM_H_INCLUDED
#include "mediainformationheaderatom.h"
#endif

class Mpeg4MediaHeaderAtom : public MediaInformationHeaderAtom
{

    public:
        Mpeg4MediaHeaderAtom(MP4_FF_FILE *fp,
                             uint32 mediaType,
                             uint32 size,
                             uint32 type);

        virtual ~Mpeg4MediaHeaderAtom();

        virtual uint32 getMediaInformationHeaderType() const;


    private:
        uint32 _mediaType; // DO NOT RENDER THIS VALUE!!! (Only for internal purposes)
};


#endif  // MPEG4MEDIAHEADERATOM_H_INCLUDED

