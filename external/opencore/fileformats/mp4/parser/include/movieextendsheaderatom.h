
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                            MPEG-4 Movie Extends Header Atom Class                             */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/

#ifndef MOVIEEXTENDSHEADERATOM_H_INCLUDED
#define MOVIEEXTENDSHEADERATOM_H_INCLUDED

#ifndef FULLATOM_H_INCLUDED
#include "fullatom.h"
#endif

class MovieExtendsHeaderAtom : public FullAtom
{

    public:
        MovieExtendsHeaderAtom(MP4_FF_FILE *fp,
                               uint32 size,
                               uint32 type);
        virtual ~MovieExtendsHeaderAtom();

        uint64 getFragmentDuration()
        {
            if (_version == 1)
            {
                return _fragmentDuration64;
            }
            else
                return _fragmentDuration32;
        }

    private:
        uint32 _fragmentDuration32;
        uint64 _fragmentDuration64;
        uint32 _version;
        uint32 _flags;
};

#endif
