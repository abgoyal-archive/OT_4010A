


#ifndef __H263SampleEntry_H__
#define __H263SampleEntry_H__

#include "sampleentry.h"
#include "decoderspecificinfo.h"
#include "h263decoderspecificinfo3gpp.h"

class PVA_FF_H263SampleEntry : public PVA_FF_SampleEntry
{

    public:

        PVA_FF_H263SampleEntry(); // Constructor

        virtual ~PVA_FF_H263SampleEntry();

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

        // Getting and setting the Mpeg4 VOL header
        virtual PVA_FF_DecoderSpecificInfo *getDecoderSpecificInfo() const
        {
            return NULL;
        }
        virtual PVA_FF_H263SpecficAtom *get3GPPDecoderSpecificInfo() const
        {
            return pH263SpecificAtom;
        }

        void setVideoParams(uint16 frame_width, uint16 frame_height)
        {
            _reserved2 = frame_width;
            _reserved2 = (_reserved2 << 16);

            _reserved2 |= frame_height;
        }

    private:
        void init();
        virtual void recomputeSize();

        // Reserved constants
        uint32 _reserved1[4]; // = { 0, 0, 0, 0 };
        uint32 _reserved2; // = 0x014000f0;
        uint32 _reserved3; // = 0x00480000;
        uint32 _reserved4; // = 0x00480000;
        uint32 _reserved5; // = 0;
        uint16 _reserved6; // = 1;
        uint8  _reserved7[32]; // = 0;
        uint16 _reserved8; // = 24;
        int16  _reserved9; // = -1; // (16) SIGNED!

        PVA_FF_H263SpecficAtom *pH263SpecificAtom;
};


#endif

