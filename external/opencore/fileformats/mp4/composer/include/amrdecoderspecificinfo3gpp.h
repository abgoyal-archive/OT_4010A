

#ifndef __AMRDecoderSpecificInfo3GPP_H__
#define __AMRDecoderSpecificInfo3GPP_H__

#include "oscl_types.h"
#include "oscl_file_io.h"

#include "atom.h"
#include "atomutils.h"
#include "a_atomdefs.h"

class PVA_FF_AMRSpecificAtom : public PVA_FF_Atom
{

    public:
        PVA_FF_AMRSpecificAtom(); // Default constructor
        virtual ~PVA_FF_AMRSpecificAtom() {}; // Destructor

        void setVendorcode(int32 VendorCode = PACKETVIDEO_FOURCC)
        {
            _VendorCode = VendorCode;
        }
        void setEncoderVersion(uint8 encoder_version = 0)
        {
            _encoder_version = encoder_version;
        }
        void setModeSet(uint16 mode_set)
        {
            _mode_set = mode_set;
        }
        void setModeChangePeriod(uint8 mode_change_period = 0)
        {
            _mode_change_period = mode_change_period;
        }
        void setFramesPerSample(uint8 frames_per_sample = 1)
        {
            _frames_per_sample = frames_per_sample;
        }

        uint16 getModeSet()
        {
            return _mode_set;
        }

        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

        virtual void recomputeSize();

    private:
        int32            _VendorCode;
        uint8            _encoder_version;
        uint16           _mode_set;
        uint8            _mode_change_period;
        uint8            _frames_per_sample;
};

#endif

