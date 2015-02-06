


#ifndef COPYRIGHTATOM_H_INCLUDED
#define COPYRIGHTATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif
#ifndef FULLATOM_H_INCLUDED
#include "fullatom.h"
#endif

class CopyRightAtom : public FullAtom
{

    public:
        CopyRightAtom(MP4_FF_FILE *fp, uint32 size, uint32 type); // Constructor
        virtual ~CopyRightAtom();

        // Member gets and sets
        uint16 getLanguageCode()
        {
            return _language_code;
        }

        OSCL_wString& getCopyRightString()
        {
            return _copyRightNotice;
        }

    private:
        uint16       _language_code; // (15bits) padded with a single bit when rendering
        OSCL_wHeapString<OsclMemAllocator>  _copyRightNotice;
};


#endif // COPYRIGHTATOM_H_INCLUDED

