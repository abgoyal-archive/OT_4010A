

#ifndef __PVA_FF_AVCSampleEntry_H__
#define __PVA_FF_AVCSampleEntry_H__

#include "atom.h"
#include "oscl_file_io.h"
#include "avcconfigurationatom.h"
#include "decoderspecificinfo.h"
#include "sampleentry.h"
#include "mpeg4bitrateatom.h"

class PVA_FF_AVCSampleEntry : public PVA_FF_SampleEntry
{

    public:
        PVA_FF_AVCSampleEntry(uint8 profile = 1, uint8 profileComp = 0xFF, uint8 level = 0xFF);
        virtual ~PVA_FF_AVCSampleEntry();
        void setVideoParam(int16 width, int16 height);
        void setBitrate(uint32 BufferSizeDB, uint32 MaxBitRate, uint32 AvgBitRate);
        void setSample(void* sample, uint32 size)
        {

            _pAVCConfigurationAtom->setSample(sample, size);
        }
        virtual void recomputeSize();
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);
        void addDecoderSpecificInfo(PVA_FF_DecoderSpecificInfo *pinfo);
        bool checkPS()
        {
            if (_pAVCConfigurationAtom->checkIfPSWritten())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    private:
        void init(uint8 profile, uint8 profileComp, uint8 level);



        // Reserved constants
        uint8  _reserved[6];
        uint16 _dataReferenceIndex;
        int16  _preDefined1;
        int16  _reserved1;
        int32  _predefined2[3];
        int16  _width;
        int16  _height;
        int32  _horizResolution;
        int32  _vertResolution;
        int32  _reserved2;
        int16  _preDefined2;
        uint8  _compressorName[32];
        int16  _depth;
        int16  _predefined3;
        int16  _predefined4;

        PVA_FF_AVCConfigurationAtom          *_pAVCConfigurationAtom;
        PVA_FF_Mpeg4Bitrate                  *_pMpeg4BitrateAtom;
};


#endif
