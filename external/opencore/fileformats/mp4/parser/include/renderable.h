
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                           MPEG-4 Renderable Class                             */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef RENDERABLE_H_INCLUDED
#define RENDERABLE_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

class Renderable
{

    public:
        virtual ~Renderable() {}

        virtual uint32 getSize() const = 0;
};

#endif // RENDERABLE_H_INCLUDED


