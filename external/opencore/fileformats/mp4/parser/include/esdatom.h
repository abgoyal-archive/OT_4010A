
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                             MPEG-4 ESDAtom Class                              */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef ESDATOM_H_INCLUDED
#define ESDATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef FULLATOM_H_INCLUDED
#include "fullatom.h"
#endif

#ifndef ESDESCRIPTOR_H_INCLUDED
#include "esdescriptor.h"
#endif

class ESDAtom : public FullAtom
{

    public:
        ESDAtom(MP4_FF_FILE *fp, uint32 size, uint32 type);
        virtual ~ESDAtom();

        // Member gets and sets
        const ESDescriptor &getESDescriptor() const
        {
            return *_pdescriptor;
        }
        const ESDescriptor *getESDescriptorPtr() const
        {
            return _pdescriptor;
        }
        uint8 getObjectTypeIndication()
        {
            if (_pdescriptor != NULL)
            {
                return  _pdescriptor->getObjectTypeIndication();
            }
            else
            {
                return 0xFF;
            }
        }

        // Getting and setting the Mpeg4 VOL header
        DecoderSpecificInfo *getDecoderSpecificInfo() const
        {
            if (_pdescriptor != NULL)
            {
                return _pdescriptor->getDecoderSpecificInfo();
            }
            else
            {
                return NULL;
            }
        }

        // Get the max size buffer needed to retrieve the media samples
        uint32 getMaxBufferSizeDB() const
        {
            if (_pdescriptor != NULL)
            {
                return _pdescriptor->getMaxBufferSizeDB();
            }
            else
            {
                return 0;
            }
        }

        uint32 getAverageBitrate() const
        {
            if (_pdescriptor != NULL)
            {
                return _pdescriptor->getAverageBitrate();
            }
            else
            {
                return 0;
            }
        }

        uint32 getMaxBitrate() const
        {
            if (_pdescriptor != NULL)
            {
                return _pdescriptor->getMaxBitrate();
            }
            else
            {
                return 0;
            }
        }

    private:

        ESDescriptor *_pdescriptor;
};


#endif // ESDATOM_H_INCLUDED


