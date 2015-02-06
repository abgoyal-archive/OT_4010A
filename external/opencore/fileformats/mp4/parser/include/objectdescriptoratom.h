
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                        MPEG-4 ObjectDescriptorAtom Class                      */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef OBJECTDESCRIPTORATOM_H_INCLUDED
#define OBJECTDESCRIPTORATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef FULLATOM_H_INCLUDED
#include "fullatom.h"
#endif

#ifndef INITIALOBJECTDESCRIPTOR_H_INCLUDED
#include "initialobjectdescriptor.h"
#endif

class ObjectDescriptorAtom : public FullAtom
{

    public:
        ObjectDescriptorAtom(MP4_FF_FILE *fp, uint32 size, uint32 type);
        virtual ~ObjectDescriptorAtom();

        // Member gets
        const InitialObjectDescriptor &getInitialObjectDescriptor() const
        {
            return *_pOD;
        }

    private:
        InitialObjectDescriptor *_pOD;
};

#endif // OBJECTDESCRIPTORATOM_H_INCLUDED

