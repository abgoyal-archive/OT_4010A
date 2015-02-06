


#ifndef AMRSAMPLEENTRY_H_INCLUDED
#define AMRSAMPLEENTRY_H_INCLUDED

#ifndef ATOM_H_INCLUDED
#include "atom.h"
#endif
#ifndef ATOMUTILS_H_INCLUDED
#include "atomutils.h"
#endif
#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif
#ifndef GPP_AMRDECODERSPECIFICINFO_H_INCLUDED
#include "3gpp_amrdecoderspecificinfo.h"
#endif
#include "oscl_vector.h"
class AMRSampleEntry : public Atom
{

    public:
        AMRSampleEntry(MP4_FF_FILE *fp, uint32 size, uint32 type); // Stream-in ctor
        virtual ~AMRSampleEntry();

        uint16 getTimeScale()
        {
            return _timeScale;
        }

        virtual AMRSpecificAtom *getDecoderSpecificInfo() const
        {
            return _pAMRSpecificAtom;
        }

        DecoderSpecificInfo *getDecoderSpecificInfo(uint32 index);

        int32 getBitRate()
        {
            if (_pAMRSpecificAtom != NULL)
            {
                return (_pAMRSpecificAtom->getBitRate());
            }
            else
            {
                return 0;
            }
        }

    private:
        // Reserved constants
        uint8 _reserved[6];
        uint16 _dataReferenceIndex;
        uint32 _reserved1[2];
        uint16 _reserved2;
        uint16 _reserved3;
        uint32 _reserved4;
        uint16 _reserved5;
        uint16 _timeScale;

        AMRSpecificAtom *_pAMRSpecificAtom;

        Oscl_Vector<DecoderSpecificInfo *, OsclMemAllocator> *_pAMRDecSpecInfoArray;
};


#endif // AMRSAMPLEENTRY_H_INCLUDED
