


#ifndef __TimeToSampleAtom_H__
#define __TimeToSampleAtom_H__

#include "fullatom.h"

#define PV_ERROR -1

class PVA_FF_TimeToSampleAtom : public PVA_FF_FullAtom
{

    public:
        PVA_FF_TimeToSampleAtom(uint32 mediaType); // Constructor
        virtual ~PVA_FF_TimeToSampleAtom();

        // Member gets and sets
        uint32 getEntryCount() const
        {
            return _entryCount;
        }

        void nextSample(uint32 ts);

        void updateLastTSEntry(uint32 ts);
        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

    private:
        void addDelta(int32 delta);
        void addEntry(uint32 count, int32 delta);
        void insertEntryAt(int32 index, uint32 count, int32 delta);
        void replaceEntryAt(int32 index, uint32 count, int32 delta);

        virtual void recomputeSize();

        uint32 _entryCount;

        Oscl_Vector<uint32, OsclMemAllocator> *_psampleCountVec;
        Oscl_Vector<int32, OsclMemAllocator>  *_psampleDeltaVec;

        int32 _mediaType;

        // For visual samples
        uint32 _currentTimestamp;

        // Flag to say when the first entry has been received
        bool _firstEntry;

        bool _lastTSUpdated;
};


#endif

