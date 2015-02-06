


#ifndef BASEDESCRIPTOR_H_INCLUDED
#define BASEDESCRIPTOR_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif
#ifndef EXPANDABLEBASECLASS_H_INCLUDED
#include "expandablebaseclass.h"
#endif

const uint32 DEFAULT_DESCRIPTOR_SIZE = 1; // 8 bits for the tag ONLY
// _sizeOfClass is computed explicitly elsewhere!

class BaseDescriptor : public ExpandableBaseClass
{

    public:
        BaseDescriptor(uint8 tag)
        {
            _tag = tag;    // DELETE THIS CTOR
        }
        BaseDescriptor(MP4_FF_FILE *fp, bool o3GPPTrack = false); // Stream-in Constructor
        virtual ~BaseDescriptor();

        uint32 getSize() const
        {
            return _sizeOfClass;
        }
        uint32 getDefaultDescriptorSize() const
        {
            return DEFAULT_DESCRIPTOR_SIZE;
        }

};

#endif // BASEDESCRIPTOR_H_INCLUDED

