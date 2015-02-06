


#ifndef __SyncSampleAtom_H__
#define __SyncSampleAtom_H__


#define PV_ERROR -1

#include "fullatom.h"


class PVA_FF_SyncSampleAtom : public PVA_FF_FullAtom
{

    public:
        PVA_FF_SyncSampleAtom(); // Constructor
        virtual ~PVA_FF_SyncSampleAtom(); // Destructor

        // Member gets and sets
        uint32 getEntryCount() const
        {
            return _entryCount;
        }

        void nextSample(uint8 flags);

        // Adding to and getting the sample number values
        void addSampleNumber(uint32 sample);

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

    private:
        virtual void recomputeSize();

        uint32 _entryCount;
        Oscl_Vector<uint32, OsclMemAllocator>* _psampleNumbers;

        uint32 _nextSampleNumber;

};


#endif

