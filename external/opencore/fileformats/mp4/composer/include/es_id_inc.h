


#ifndef __ES_ID_Inc_H__
#define __ES_ID_Inc_H__

#include "basedescriptor.h"

class PVA_FF_ES_ID_Inc : public PVA_FF_BaseDescriptor
{

    public:
        PVA_FF_ES_ID_Inc(uint32 trackid); // Alternate constructor
        virtual ~PVA_FF_ES_ID_Inc(); // Destructor

        // Member gets and sets
        uint16 getTrackID() const
        {
            return (uint16)(_trackID);
        }
        void setTrackID(uint32 trackid)
        {
            _trackID = trackid;
        }

        // Recompute the size of the class as is rendered to a file string
        virtual void recomputeSize();

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

    private:
        uint32 _trackID; // (32)

};



#endif

