
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                           MPEG-4 Util: IMediaBuffer                           */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef IMEDIABUFFER_H_INCLUDED
#define IMEDIABUFFER_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

class IMediaBuffer
{

    public:
        virtual ~IMediaBuffer() {}; // So destructor of MediaBuffer gets called

        // Member get methods

        // Returns the size of the data in the buffer
        virtual uint32 getSize() const = 0;

        // Returns a pointer to the data in the buffer
        virtual uint8 *getDataPtr() = 0;
        // Returns a read-only pointer to the data in the buffer - USE THIS METHOD
        virtual uint8 *getReadOnlyDataPtr() const = 0;

        // This method MUST be called when you are done with the buffer.  This allows
        // the underlying buffer manager to handle memory management by reusing the
        // buffer when you are done.
        virtual void freeBuf() = 0;

};

#endif  // IMEDIABUFFER_H_INCLUDED

