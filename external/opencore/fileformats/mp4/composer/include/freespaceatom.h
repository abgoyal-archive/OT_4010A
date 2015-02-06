


#ifndef __FreeSpaceAtom_H__
#define __FreeSpaceAtom_H__

#include "atom.h"


class PVA_FF_FreeSpaceAtom : public PVA_FF_Atom
{

    public:
        PVA_FF_FreeSpaceAtom(); // Constructor
        PVA_FF_FreeSpaceAtom(PVA_FF_FreeSpaceAtom atom); // Copy Constructor
        virtual ~PVA_FF_FreeSpaceAtom();

        // Adding empty data
        void addData(); // Data doesn't matter so no need to pass in any value
        Oscl_Vector<uint8, OsclMemAllocator>* getData()
        {
            return _pdata;
        }

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

    private:
        Oscl_Vector<uint8, OsclMemAllocator>* _pdata;

};


#endif

