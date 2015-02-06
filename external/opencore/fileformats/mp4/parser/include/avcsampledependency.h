
#ifndef __AVCSampleDependency_H__
#define __AVCSampleDependency_H__

#include "atom.h"
#include "atomutils.h"
#include "oscl_file_io.h"
#include "fullatom.h"


class AVCSampleDependency : public FullAtom
{

    public:

        AVCSampleDependency(MP4_FF_FILE *fp, uint32 size, uint32 type, uint32 sample_count);

        virtual ~AVCSampleDependency();

        uint16 getDependencyCount(uint32 index)
        {
            return _dependencycount[index];
        }

        uint16 getRelativeSampleNumber(uint32 index)
        {

            return _relativesamplenumber[index];
        }

    private:
        uint16 *_dependencycount;
        uint16 *_relativesamplenumber;
};
#endif
