
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                         MPEG-4 MpegSampleEntry Class                          */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef MPEGSAMPLEENTRY_H_INCLUDED
#define MPEGSAMPLEENTRY_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef SAMPLEENTRY_H_INCLUDED
#include "sampleentry.h"
#endif

#ifndef ESDATOM_H_INCLUDED
#include "esdatom.h"
#endif

class MpegSampleEntry : public SampleEntry
{

    public:
        MpegSampleEntry(MP4_FF_FILE *fp, uint32 size, uint32 type);
        virtual ~MpegSampleEntry();

        uint8 getObjectTypeIndication() const
        {
            if (_pes != NULL)
            {
                return _pes->getObjectTypeIndication();
            }
            else
            {
                return 0xFF;
            }
        }

        // Member gets and sets
        const ESDAtom &getESDAtom() const
        {
            return *_pes;
        }

        virtual uint32 getESID() const
        {
            if (_pes != NULL)
            {
                if (_pes->getESDescriptorPtr() != NULL)
                {
                    return _pes->getESDescriptorPtr()->getESID();
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }

        virtual const ESDescriptor *getESDescriptor() const
        {
            if (_pes != NULL)
            {
                return _pes->getESDescriptorPtr();
            }
            else
            {
                return NULL;
            }
        }

    private:
        ESDAtom *_pes;
};


#endif  // MPEGSAMPLEENTRY_H_INCLUDED

