
#ifndef __AVCSampleToGroup_H__
#define __AVCSampleToGroup_H__

#include "atom.h"
#include "atomutils.h"
#include "oscl_file_io.h"
#include "fullatom.h"


class AVCSampleToGroup: public FullAtom
{
    public:

        AVCSampleToGroup(MP4_FF_FILE *fp,
                         uint32 size, uint32 type);
        virtual ~AVCSampleToGroup();
        uint32 getGroupingType()
        {
            return _grouping_type;
        }
        uint32 getGroupDescriptionIndex(uint32 index)
        {
            return _group_description_indexVec[index];
        }

    private:
        uint32 _grouping_type;
        uint32 _entry_count;
        uint32 * _samplecountVec;
        uint32 * _group_description_indexVec;
};
#endif
