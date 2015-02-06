
#ifndef MPEG4BITRATE_H_INCLUDED
#define MPEG4BITRATE_H_INCLUDED

#include "atom.h"
#include "atomutils.h"
#include "oscl_file_io.h"

class PVA_FF_Mpeg4Bitrate : public PVA_FF_Atom
{

    public:
        PVA_FF_Mpeg4Bitrate(uint32 BufferSizeDB, uint32 MaxBitRate, uint32 AvgBitRate);
        virtual ~PVA_FF_Mpeg4Bitrate() {};  // Destructor
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);
        virtual void recomputeSize();
        virtual uint32 getSize();

    private:
        uint32 _bufferSizeDB;
        uint32 _maxBitRate;
        uint32 _avgBitRate;
};

#endif // MPEG4BITRATE_H_INCLUDED
