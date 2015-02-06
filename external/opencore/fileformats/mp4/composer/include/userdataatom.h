


#ifndef __UserDataAtom_H__
#define __UserDataAtom_H__

#include "atom.h"
#include "a_isucceedfail.h"
#include "atomutils.h"


class PVA_FF_UserDataAtom : public PVA_FF_Atom, public PVA_FF_ISucceedFail
{

    public:
        PVA_FF_UserDataAtom();              // Constructor
        virtual ~PVA_FF_UserDataAtom();

        void addAtom(PVA_FF_Atom* atom);

        int32 getUserDataAtomVecSize()
        {
            if (_pUserDataAtomVec != NULL)
            {
                return (_pUserDataAtomVec->size());
            }
            return 0;
        }

        virtual void recomputeSize();

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);


    private:
        Oscl_Vector<PVA_FF_Atom*, OsclMemAllocator> *_pUserDataAtomVec; //Array of User Data PVA_FF_Atom Pointers

};

#endif

