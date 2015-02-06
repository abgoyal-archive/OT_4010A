
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                          MPEG-4 ES_ID_Inc Class                               */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef ES_ID_INC_H_INCLUDED
#define ES_ID_INC_H_INCLUDED

#ifndef BASEDESCRIPTOR_H_INCLUDED
#include "basedescriptor.h"
#endif

class ES_ID_Inc : public BaseDescriptor
{

    public:
        ES_ID_Inc(MP4_FF_FILE *fp); // Stream-in constructor
        virtual ~ES_ID_Inc(); // Destructor

        // Member gets and sets
        uint32 getTrackID() const
        {
            return _trackID;
        }


    private:
        uint32  _trackID; // (32)

};



#endif // ES_ID_INC_H_INCLUDED

