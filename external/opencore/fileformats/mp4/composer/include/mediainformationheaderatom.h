


#ifndef __MediaInformationAtomHeader_H__
#define __MediaInformationAtomHeader_H__

#include "fullatom.h"

class PVA_FF_MediaInformationHeaderAtom : public PVA_FF_FullAtom
{

    public:
        PVA_FF_MediaInformationHeaderAtom(uint32 atomType, uint8 version, uint32 flags); // Constructor
        virtual ~PVA_FF_MediaInformationHeaderAtom();

        // Method to get the header type
        virtual uint32 getMediaInformationHeaderType() const = 0;

};



#endif

