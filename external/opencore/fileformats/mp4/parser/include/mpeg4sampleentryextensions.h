
#ifndef __MPEG4SampleEntryExtensions_H__
#define __MPEG4SampleEntryExtensions_H__

#include "atom.h"
#include "atomutils.h"
#include "oscl_file_io.h"

class MPEG4BitRateBox : public Atom
{
    public:
        MPEG4BitRateBox(MP4_FF_FILE *fp, uint32 size, uint32 type);
        virtual ~MPEG4BitRateBox() {};

        uint32 getBufferSizeDB()
        {
            return _bufferSizeDB;
        }
        uint32 getMaxBitRate()
        {
            return _maxBitRate;
        }
        uint32 getAvgBitRate()
        {
            return _avgBitRate;
        }

    private:
        uint32 _bufferSizeDB;
        uint32 _maxBitRate;
        uint32 _avgBitRate;
};

#endif
