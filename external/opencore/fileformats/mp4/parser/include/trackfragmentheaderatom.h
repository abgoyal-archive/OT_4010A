
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                            MPEG-4 Track Fragment Header Atom Class                             */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/

#ifndef TRACKFRAGMENTHEADERATOM_H_INCLUDED
#define TRACKFRAGMENTHEADERATOM_H_INCLUDED

#ifndef FULLATOM_H_INCLUDED
#include "fullatom.h"
#endif

class TrackFragmentHeaderAtom : public FullAtom
{

    public:
        TrackFragmentHeaderAtom(MP4_FF_FILE *fp,
                                uint32 size,
                                uint32 type);
        virtual ~TrackFragmentHeaderAtom();

        uint64 getBaseDataOffset()
        {
            return _base_data_offset;
        }
        uint32 getSampleDescriptionIndex()
        {
            return _sample_description_index;
        }

        uint32 getDefaultSampleDuration()
        {
            return _default_sample_duration;
        }

        uint32 getDefaultSampleSize()
        {
            return _default_sample_size;
        }

        uint32 getDefaultSampleFlag()
        {
            return _default_sample_flag;
        }

        uint32 getTrackId()
        {
            return _trackID;
        }

    private:
        uint32 tf_flag;
        uint32 _trackID;
        uint64 _base_data_offset;
        uint32 _sample_description_index;
        uint32 _default_sample_duration;
        uint32 _default_sample_size;
        uint32 _default_sample_flag;
        uint32 _version;
};

#endif
