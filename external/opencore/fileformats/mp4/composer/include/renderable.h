


#ifndef __Renderable_H__
#define __Renderable_H__

#include "atomutils.h"

class PVA_FF_Renderable
{

    public:
        virtual ~PVA_FF_Renderable() {}

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ofstream
        // Each subclass will override this method to render its own contents.
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp) = 0;

        virtual uint32 getSize() const = 0;

        virtual MP4_AUTHOR_FF_FILE_IO_WRAP* dump(MP4_AUTHOR_FF_FILE_IO_WRAP *fp)
        {
            return fp;
        }

};

class PVA_FF_BufferHolder : public PVA_FF_Renderable
{

    public:
        PVA_FF_BufferHolder(uint8* buf, int32 size)
        {
            _dataBuffer = NULL;
            _dataSize   = 0;

            if (size > 0)
            {
                _dataBuffer = (uint8*)(oscl_malloc(sizeof(uint8) * size));
                oscl_memcpy(_dataBuffer, buf, size);
                _dataSize   = size;
            }
        };

        virtual ~PVA_FF_BufferHolder()
        {
            if (_dataBuffer != NULL)
            {
                oscl_free(_dataBuffer);
            }
        }

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ofstream
        // Each subclass will override this method to render its own contents.
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp)
        {
            if (_dataBuffer != NULL)
            {
                if (!PVA_FF_AtomUtils::renderByteData(fp, (uint32)_dataSize, _dataBuffer))
                {
                    return false;
                }
            }
            return true;
        };

        virtual uint32 getSize() const
        {
            return _dataSize;
        }

    private:
        uint8*   _dataBuffer;
        int32    _dataSize;

};


#endif

