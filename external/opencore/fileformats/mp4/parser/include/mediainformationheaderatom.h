
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                   MPEG-4 MediaInformationHeaderAtom Class                     */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef MEDIAINFORMATIONHEADERATOM_H_INCLUDED
#define MEDIAINFORMATIONHEADERATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef FULLATOM_H_INCLUDED
#include "fullatom.h"
#endif


class MediaInformationHeaderAtom : public FullAtom
{

    public:
        MediaInformationHeaderAtom(MP4_FF_FILE *fp, uint32 size, uint32 type); // Stream-in ctor
        virtual ~MediaInformationHeaderAtom();

        // Method to get the header type
        virtual uint32 getMediaInformationHeaderType() const = 0;
};

#endif // MEDIAINFORMATIONHEADERATOM_H_INCLUDED


