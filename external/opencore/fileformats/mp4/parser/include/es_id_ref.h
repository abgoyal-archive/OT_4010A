
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                          MPEG-4 ES_IF_Ref Class                               */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/

#ifndef ES_ID_REF_H_INCLUDED
#define ES_ID_REF_H_INCLUDED

#ifndef BASEDESCRIPTOR_H_INCLUDED
#include "basedescriptor.h"
#endif

class ES_ID_Ref : public BaseDescriptor
{
    public:
        ES_ID_Ref(MP4_FF_FILE *fp);  // Stream-in constructor
        virtual ~ES_ID_Ref(); // Destructor

        // Member gets and sets
        uint16 getTrackIndex() const
        {
            return _trackIndex;
        }


    private:
        uint16 _trackIndex; // (16)

};



#endif // ES_ID_REF_H_INCLUDED


