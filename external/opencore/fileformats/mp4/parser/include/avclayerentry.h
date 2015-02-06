
#ifndef __AVCLayerDescEntry_H__
#define __AVCLayerDescEntry_H__

#include "atom.h"
#include "atomutils.h"
#include "oscl_file_io.h"
#include "visualsampleentry.h"


class AVCLayerDescEntry : public VisualSampleEntry
{

    public:
        AVCLayerDescEntry(MP4_FF_FILE *fp, uint32 size, uint32 type);
        virtual ~AVCLayerDescEntry();

        uint8 getlayerNumber()
        {
            return _layerNumber;
        }

        uint8 getAccurateStatisticsFlag()
        {
            return _accurateStatisticsFlag;
        }

        uint16 getAvgBitRate()
        {
            return _avgBitRate;
        }

        uint16 getAvgFrameRate()
        {
            return _avgFrameRate;
        }


    private:
        uint8 _layerNumber;
        uint32 _duration;
        uint8 _accurateStatisticsFlag;
        uint16 _avgBitRate;
        uint16 _avgFrameRate;
};
#endif
