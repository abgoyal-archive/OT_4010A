


#ifndef __DataReferenceAtom_H__
#define __DataReferenceAtom_H__

#include "fullatom.h"
#include "dataentryatom.h"

class PVA_FF_DataReferenceAtom : public PVA_FF_FullAtom
{

    public:
        PVA_FF_DataReferenceAtom(); // Constructor
        virtual ~PVA_FF_DataReferenceAtom();

        // Member gets and sets
        uint32 getEntryCount() const
        {
            return _entryCount;
        }

        // Adding atoms to and getting the vector of DataEntryAtoms
        void addDataEntryAtom(PVA_FF_DataEntryAtom *atom);

        Oscl_Vector<PVA_FF_DataEntryAtom*, OsclMemAllocator>& getDataEntryVec() const
        {
            return *_pdataEntryVec;
        }
        Oscl_Vector<PVA_FF_DataEntryAtom*, OsclMemAllocator>& getMutableDataEntryVec()
        {
            return *_pdataEntryVec;
        }

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

    private:
        void init();
        virtual void recomputeSize();

        uint32 _entryCount;
        Oscl_Vector<PVA_FF_DataEntryAtom*, OsclMemAllocator> *_pdataEntryVec; // Careful with vector access and cleanup!

};


#endif

