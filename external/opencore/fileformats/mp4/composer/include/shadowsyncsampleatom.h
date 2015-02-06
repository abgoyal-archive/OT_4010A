


#ifndef __ShadowSyncSampleAtom_H__
#define __ShadowSyncSampleAtom_H__

#include "fullatom.h"


class PVA_FF_ShadowSyncSampleAtom : public PVA_FF_FullAtom
{

    public:
        PVA_FF_ShadowSyncSampleAtom(uint8 version, uint32 flags); // Constructor
        PVA_FF_ShadowSyncSampleAtom(PVA_FF_ShadowSyncSampleAtom atom); // Copy Constructor
        virtual ~PVA_FF_ShadowSyncSampleAtom();

        // Member gets and sets
        uint32 getEntryCount()
        {
            return _entryCount;
        }

        // Adding to and getting the shadow and sync sample number values
        void addShadowSampleNumber(uint32 sample);
        Oscl_Vector<uint32, OsclMemAllocator>* getShadowSampleNumbers()
        {
            return _pshadowSampleNumbers;
        }
        uint32 getShadowSampleNumberAt(int index);

        void addSyncSampleNumber(uint32 sample);
        Oscl_Vector<uint32, OsclMemAllocator>* getSyncSampleNumbers()
        {
            return _psyncSampleNumbers;
        }
        uint32 getSyncSampleNUmberAt(int index);

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

    private:
        uint32 _entryCount;
        Oscl_Vector<uint32, OsclMemAllocator>* _pshadowSampleNumbers;
        Oscl_Vector<uint32, OsclMemAllocator>* _psyncSampleNumbers;

};


#endif

