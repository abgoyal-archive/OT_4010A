
#ifndef GPP_AMRDECODERSPECIFICINFO_H_INCLUDED
#define GPP_AMRDECODERSPECIFICINFO_H_INCLUDED

#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif

#ifndef ATOM_H_INCLUDED
#include "atom.h"
#endif

#ifndef ATOMDEFS_H_INCLUDED
#include "atomdefs.h"
#endif

#ifndef DECODERSPECIFICINFO_H_INCLUDED
#include "decoderspecificinfo.h"
#endif


class AMRSpecificAtom : public Atom
{

    public:
        AMRSpecificAtom(MP4_FF_FILE *fp, uint32 size, uint32 type); // Default constructor
        virtual ~AMRSpecificAtom() {}; // Destructor

        uint32 getVendorcode()
        {
            return _VendorCode;
        }
        uint8  getDecoderVersion()
        {
            return _decoder_version;
        }
        uint16 getModeSet()
        {
            return _mode_set;
        }
        uint8  getModeChangePeriod()
        {
            return _mode_change_period;
        }
        uint8  getFramesPerSample()
        {
            return _frames_per_sample;
        }
        int32  getBitRate()
        {
            return _bitRate;
        }

    private:
        uint32        _VendorCode;
        uint8         _decoder_version;
        uint16        _mode_set;
        uint8         _mode_change_period;
        uint8         _frames_per_sample;
        int32         _bitRate;
};

#endif // GPP_AMRDECODERSPECIFICINFO_H_INCLUDED
