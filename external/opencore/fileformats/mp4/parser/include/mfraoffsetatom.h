
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                            MPEG-4 Movie Fragment Random Access Offset Atom Class                             */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/

#ifndef MOVIEFRAGMENTRANDOMACCESSOFFSETATOM_H_INCLUDED
#define MOVIEFRAGMENTRANDOMACCESSOFFSETATOM_H_INCLUDED

#ifndef FULLATOM_H_INCLUDED
#include "fullatom.h"
#endif

#ifndef ATOMUTILS_H_INCLUDED
#include "atomutils.h"
#endif


class MfraOffsetAtom : public FullAtom
{

    public:
        MfraOffsetAtom(MP4_FF_FILE *fp,
                       uint32 size,
                       uint32 type);

        virtual ~MfraOffsetAtom();

        uint32 getSizeStoredInmfro()
        {
            return _size;
        }
    private:
        uint32 _size;
        PVLogger *iLogger, *iStateVarLogger, *iParsedDataLogger;
};

#endif
