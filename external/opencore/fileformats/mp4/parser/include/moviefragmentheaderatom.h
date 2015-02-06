
#ifndef MOVIEFRAGMENTHEADERATOM_H_INCLUDED
#define MOVIEFRAGMENTHEADERATOM_H_INCLUDED

#ifndef FULLATOM_H_INCLUDED
#include "fullatom.h"
#endif

class MovieFragmentHeaderAtom : public FullAtom
{

    public:
        MovieFragmentHeaderAtom(MP4_FF_FILE *fp,
                                uint32 size,
                                uint32 type);
        virtual ~MovieFragmentHeaderAtom();
        int32 getSequenceNumber()
        {
            return _sequence_number;
        }

    private:
        uint32 _sequence_number;
        uint32 _version;
        uint32 _flags;
};

#endif
