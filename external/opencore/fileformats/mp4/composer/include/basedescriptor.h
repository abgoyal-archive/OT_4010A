


#ifndef __BaseDescriptor_H__
#define __BaseDescriptor_H__

#include "oscl_types.h"

#include "expandablebaseclass.h"

const uint32 DEFAULT_DESCRIPTOR_SIZE = 1; // 8 bits for the tag ONLY
// _sizeOfClass is computed explicitly elsewhere!

class PVA_FF_BaseDescriptor : public PVA_FF_ExpandableBaseClass
{

    public:
        PVA_FF_BaseDescriptor(uint8 tag); // Constructor
        virtual ~PVA_FF_BaseDescriptor();

        // Rendering only the members of the PVA_FF_BaseDescriptor class
        int renderBaseDescriptorMembers(MP4_AUTHOR_FF_FILE_IO_WRAP *fp) const;

        virtual void recomputeSize() = 0; // Should get overridden
        uint32 getSize() const
        {
            return _sizeOfClass;
        }
        uint32 getDefaultDescriptorSize() const
        {
            return DEFAULT_DESCRIPTOR_SIZE;
        }

    private:
        PVA_FF_BaseDescriptor() {} // Disabling public default constructor

};



#endif

