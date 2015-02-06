
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                           MPEG-4 SampleEntry Class                            */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef SAMPLEENTRY_H_INCLUDED
#define SAMPLEENTRY_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef ATOM_H_INCLUDED
#include "atom.h"
#endif

class ESDescriptor;
class DecoderSpecificInfo;

class SampleEntry : public Atom
{

    public:
        SampleEntry(MP4_FF_FILE *fp, uint32 size, uint32 type);
        virtual ~SampleEntry();

        // Member gets and sets
        uint16 getDataReferenceIndex() const
        {
            return _dataReferenceIndex;
        }

        virtual uint32 getESID() const
        {
            return 0;    // Should get overridden
        }
        virtual const ESDescriptor *getESDescriptor() const
        {
            return NULL;    // Should get overridden
        }

        // Getting and setting the Mpeg4 VOL header
        virtual DecoderSpecificInfo *getDecoderSpecificInfo() const ;
        virtual uint8 getObjectTypeIndication() const ;
        // Get the max size buffer needed to retrieve the media samples
        virtual uint32 getMaxBufferSizeDB() const;
        virtual uint32 getAverageBitrate() const;
        virtual uint32 getMaxBitrate() const;
        virtual uint16 getWidth() const;
        virtual uint16 getHeight() const;


    protected:
        // Reserved constants
        uint8 _reserved[6];
        uint16 _dataReferenceIndex;
};

#endif // SAMPLEENTRY_H_INCLUDED


