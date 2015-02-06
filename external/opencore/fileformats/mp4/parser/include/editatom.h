
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                              MPEG-4 EditAtom Class                            */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef EDITATOM_H_INCLUDED
#define EDITATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef ATOM_H_INCLUDED
#include "atom.h"
#endif

#ifndef EDITLISTATOM_H_INCLUDED
#include "editlistatom.h"
#endif

class EditAtom : public Atom
{

    public:
        EditAtom(MP4_FF_FILE *fp, uint32 size, uint32 type); // Copy Constructor
        uint32 getInitialTimeOffset();

        virtual ~EditAtom();

    private:
        Oscl_Vector<EditListAtom *, OsclMemAllocator> * _pEditListVec;

};


#endif // EDITATOM_H_INCLUDED


