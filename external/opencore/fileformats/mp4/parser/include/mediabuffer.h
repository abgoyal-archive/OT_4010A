
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                            MPEG-4 Util: MediaBuffer                           */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef MEDIABUFFER_H_INCLUDED
#define MEDIABUFFER_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef IMEDIABUFFER_H_INCLUDED
#include "imediabuffer.h"
#endif

#ifndef MEDIABUFFERMANAGER_H_INCLUDED
#include "mediabuffermanager.h"
#endif

#ifndef ATOMUTILS_H_INCLUDED
#include "atomutils.h"
#endif


class MediaBufferManager; // Forward lookahead for circular includes


class MediaBuffer : public IMediaBuffer
{

    public:
        MediaBuffer(int32 size); // Constructor
        virtual ~MediaBuffer(); // Destructor

        // Member set method
        void setSize(uint32 size)
        {
            _size = size;
        }

        // Member get methods
        virtual uint32 getSize() const
        {
            return _size;
        }
        virtual uint8 *getDataPtr()
        {
            return _pdata;
        }
        virtual uint8 *getReadOnlyDataPtr() const
        {
            return _pdata;
        }

        virtual void freeBuf();
        void setOwner(MediaBufferManager *);

        void readByteData(MP4_FF_FILE *fp, uint32 size);

    private:

        uint8 *_pdata;
        uint32 _size;

        MediaBufferManager *_owner;
};

#endif // MEDIABUFFER_H_INCLUDED


