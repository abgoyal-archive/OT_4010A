
#ifndef ITUNESMETADATAATOM_H_INCLUDED
#define ITUNESMETADATAATOM_H_INCLUDED

typedef Oscl_Vector<OSCL_StackString<128>, OsclMemAllocator> OSCL_StackStringVector;
/** Shared pointer of a key-value pair */
typedef OsclSharedPtr<PvmiKvp> PvmiKvpSharedPtr;

/** Vector of shared pointer of a key-value pair */
typedef Oscl_Vector<PvmiKvpSharedPtr, OsclMemAllocator> PvmiKvpSharedPtrVector;


#include "atom.h"
#include "itunesilstmetadataatom.h"

class ITunesMetaDataAtom: public Atom
{
    public:
        ITunesMetaDataAtom(MP4_FF_FILE *fp, uint32 size, uint32 type);
        virtual ~ITunesMetaDataAtom();



        OSCL_wHeapString<OsclMemAllocator> getmdirapplData()
        {
            return _mdirapplData;
        }

        PVMFStatus getMetaDataValues(OSCL_StackStringVector* aRequiredKeys,
                                     PvmiKvpSharedPtrVector& aMetaDataKVPVector);

        ITunesILstMetaDataAtom* getITunesILstMetaDataAtom()
        {
            return _pITunesILstMetaDataAtom;
        }

    private:

        // Whether this file is an M4A file or not. (By using "hdlr" tag)


        OSCL_wHeapString<OsclMemAllocator> _mdirapplData;

        // User ilst Data
        ITunesILstMetaDataAtom* _pITunesILstMetaDataAtom;


};


#endif // ITUNESMETADATAATOM_H_INCLUDED

