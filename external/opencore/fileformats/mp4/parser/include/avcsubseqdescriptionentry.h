
#ifndef __AVCSubSeqDescriptionEntry_H__
#define __AVCSubSeqDescriptionEntry_H__

#include "atom.h"
#include "atomutils.h"
#include "oscl_file_io.h"


class DependencyInfo
{

    public:
        DependencyInfo(MP4_FF_FILE *fp);
        ~DependencyInfo() {};

        uint8 getSubSeqCount()
        {
            return _subSeqCount;
        }

        uint8 getlayerNumber()
        {
            return _layerNumber;
        }

        uint16 getSubSequenceIdentifier()
        {
            return _subSequenceIdentifier;
        }

    private:
        uint8   _subSeqCount;
        uint8 _layerNumber;
        uint16 _subSequenceIdentifier;
};



class AVCSubSequenceEntry : public Atom
{

    public:
        AVCSubSequenceEntry(MP4_FF_FILE *fp, uint32 size, uint32 type);
        virtual ~AVCSubSequenceEntry();

        uint16 getSubSequenceIdentifier()
        {
            return _subSequenceIdentifier;
        }

        uint8 getlayerNumber()
        {
            return _layerNumber;
        }

        uint8 getDurationFlag()
        {
            return _durationFlag;
        }

        uint8 getAvgRateFlag()
        {
            return _avgRateFlag;
        }

        uint32 getDuration()
        {
            if (_durationFlag)
                return _duration;
            return 0;
        }

        uint8 getAccurateStatisticsFlag()
        {
            if (_avgRateFlag)
                return _accurateStatisticsFlag;
            return 0;
        }

        uint16 getAvgBitRate()
        {
            if (_avgRateFlag)
                return _avgBitRate;
            return 0;
        }

        uint16 getAvgFrameRate()
        {
            if (_avgRateFlag)
                return _avgFrameRate;
            return 0;
        }


    private:
        uint16 _subSequenceIdentifier;
        uint8 _layerNumber;
        uint8 _durationFlag;
        uint8 _avgRateFlag;
        uint8 _reserved;
        uint32 _duration;
        uint8 _accurateStatisticsFlag;
        uint16 _avgBitRate;
        uint16 _avgFrameRate;
        uint8 numReferences;
        DependencyInfo *dependencyinfo;


};


#endif
