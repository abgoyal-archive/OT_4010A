
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                          MPEG-4 HandlerAtom Class                             */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef HANDLERATOM_H_INCLUDED
#define HANDLERATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef FULLATOM_H_INCLUDED
#include "fullatom.h"
#endif

class HandlerAtom : public FullAtom
{

    public:
        HandlerAtom(MP4_FF_FILE *fp, uint32 size, uint32 type); // Stream-in ctor
        virtual ~HandlerAtom();

        // Private member gets
        uint32 getHandlerType() const
        {
            return _handlerType;
        }
        OSCL_wString& getName()
        {
            return _name;
        }

    private:

        uint32 reserved1;
        uint32 _handlerType;
        OSCL_wHeapString<OsclMemAllocator> _name;
};


#endif // HANDLERATOM_H_INCLUDED


