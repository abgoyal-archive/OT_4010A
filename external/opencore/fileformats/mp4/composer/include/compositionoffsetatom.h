

#ifndef __CompositionOffsetAtom_H__
#define __CompositionOffsetAtom_H__

#include "fullatom.h"


class PVA_FF_CompositionOffsetAtom : public PVA_FF_FullAtom
{

    public:
        PVA_FF_CompositionOffsetAtom(uint8 version, uint32 flags); // Constructor

        virtual ~PVA_FF_CompositionOffsetAtom();

        // Member gets and sets
        uint32 getEntryCount() const
        {
            return _entryCount;
        }
        void setEntryCount(uint32 count)
        {
            _entryCount = count;
        }

        void addSampleCount(uint32 count);
        vector<uint32>* getSampleCounts()
        {
            return _psampleCounts;
        }

        void addSampleOffset(uint32 offset);
        vector<uint32>* getSampleOffsets()
        {
            return _spampleOffsets;
        }
        uint32 getSampleOffsetAt(int index);

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

    private:
        uint32 _entryCount;
        vector<uint32>* _psampleCounts;
        vector<uint32>* _psampleOffsets;
};


#endif

