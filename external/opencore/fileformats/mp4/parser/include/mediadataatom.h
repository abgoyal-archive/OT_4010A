
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                          MPEG-4 MediaDataAtom Class                           */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef MEDIADATAATOM_H_INCLUDED
#define MEDIADATAATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef ATOM_H_INCLUDED
#include "atom.h"
#endif

#ifndef ATOMDEFS_H_INCLUDED
#include "atomdefs.h"
#endif

#ifndef RENDERABLE_H_INCLUDED
#include "renderable.h"
#endif

#ifndef TRACKATOM_H_INCLUDED
#include "trackatom.h"
#endif

class MediaDataAtom : public Atom
{

    public:
        MediaDataAtom(MP4_FF_FILE *fp, OSCL_wString& filename);
        virtual ~MediaDataAtom();

    private:
        int32 _type;
};

#endif // MEDIADATAATOM_H_INCLUDED


