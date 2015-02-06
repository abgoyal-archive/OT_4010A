
#ifndef FONTTABLEATOM_H_INCLUDED
#define FONTTABLEATOM_H_INCLUDED

#ifndef FONTRECORD_H_INCLUDED
#include "fontrecord.h"
#endif

#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif


#ifndef ATOM_H_INCLUDED
#include "atom.h"
#endif

#ifndef ATOMDEFS_H_INCLUDED
#include "atomdefs.h"
#endif

class FontTableAtom : public Atom
{

    public:
        FontTableAtom(MP4_FF_FILE *fp);  // Default constructor
        virtual ~FontTableAtom();  // Destructor

        uint16  getFontListSize()
        {
            return _entryCount;
        }

        FontRecord * getFontRecordAt(uint16 index);


    private:
        uint16 _entryCount;
        Oscl_Vector<FontRecord *, OsclMemAllocator> *_pFontRecordArray;
};


#endif // FONTTABLEATOM_H_INCLUDED


