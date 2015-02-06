
#ifndef __AVCSampleDependencyType_H__
#define __AVCSampleDependencyType_H__

#include "atom.h"
#include "atomutils.h"
#include "oscl_file_io.h"
#include "fullatom.h"


class AVCSampleDependencyType: public FullAtom
{
    public:

        AVCSampleDependencyType(MP4_FF_FILE *fp,
                                uint32 size, uint32 type, uint32 sample_count);
        virtual ~AVCSampleDependencyType();

    private:
        uint8 *_reserved;
        uint8 *_sample_depends_on;
        uint8 *_sample_is_depended_on;
        uint8 _version;
        uint32 flags;
};
#endif
