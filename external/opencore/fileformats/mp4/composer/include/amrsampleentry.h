


#ifndef __AMRSampleEntry_H__
#define __AMRSampleEntry_H__

#include "sampleentry.h"
#include "amrdecoderspecificinfo3gpp.h"

class PVA_FF_AMRSampleEntry : public PVA_FF_SampleEntry
{
    public:
        PVA_FF_AMRSampleEntry(uint32 format); // Constructor
        virtual ~PVA_FF_AMRSampleEntry();

        uint16 getTimeScale()
        {
            return _timeScale;
        }
        void setTimeScale(uint16 ts)
        {
            _timeScale = ts;
        }

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

        void setModeSet(uint16 mode_set)
        {
            if (pAMRSpecificAtom != NULL)
            {
                pAMRSpecificAtom->setModeSet(mode_set);
            }
        }

        uint16 getModeSet()
        {
            if (pAMRSpecificAtom != NULL)
            {
                return (pAMRSpecificAtom->getModeSet());
            }
            else
            {
                return 0;
            }
        }

        void setAudioEncodeParams(PVMP4FFComposerAudioEncodeParams &audioParams)
        {
            _reserved2 = audioParams.numberOfChannels;
            _reserved3 = audioParams.bitsPerSample;
        }

    private:
        virtual void recomputeSize();
        void init();

        // Reserved constants
        uint32 _reserved1[2]; // = { 0, 0 };
        uint16 _reserved2; // = 2;
        uint16 _reserved3; // = 16;
        uint32 _reserved4; // = 0;
        uint16 _reserved5; // = 0;

        uint16 _timeScale;

        PVA_FF_AMRSpecificAtom *pAMRSpecificAtom;
};


#endif

