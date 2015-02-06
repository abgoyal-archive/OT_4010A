
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                        MPEG-4 VisualSampleEntry Class                         */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef VISUALSAMPLEENTRY_H_INCLUDED
#define VISUALSAMPLEENTRY_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef SAMPLEENTRY_H_INCLUDED
#include "sampleentry.h"
#endif

#ifndef ESDATOM_H_INCLUDED
#include "esdatom.h"
#endif


class VisualSampleEntry : public SampleEntry
{

    public:
        VisualSampleEntry(MP4_FF_FILE *fp, uint32 size, uint32 type);
        virtual ~VisualSampleEntry();

        // Member gets and sets
        const ESDAtom &getESDAtom() const
        {
            return *_pes;
        }

        virtual uint32 getESID() const;

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

        // Getting and setting the Mpeg4 VOL header
        virtual DecoderSpecificInfo *getDecoderSpecificInfo() const
        {
            if (_pes != NULL)
            {
                return _pes->getDecoderSpecificInfo();
            }
            else
            {
                return NULL;
            }
        }

        virtual uint8 getObjectTypeIndication() const
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


        // Get the max size buffer needed to retrieve the media samples
        uint32 getMaxBufferSizeDB() const
        {
            if (_pes != NULL)
            {
                return _pes->getMaxBufferSizeDB();
            }
            else
            {
                return 0;
            }
        }

        uint32 getAverageBitrate() const
        {
            if (_pes != NULL)
            {
                return _pes->getAverageBitrate();
            }
            else
            {
                return 0;
            }
        }

        uint32 getMaxBitrate() const
        {
            if (_pes != NULL)
            {
                return _pes->getMaxBitrate();
            }
            else
            {
                return 0;
            }
        }
        uint16 getWidth() const;

        uint16 getHeight() const
        {
            return (uint16)(_reserved2 & 0x0000FFFF);

        }


    private:
        // Reserved constants
        uint32 _reserved1[4]; // = { 0, 0, 0, 0 };
        uint32 _reserved2; // = 0x014000f0;
        uint32 _reserved3; // = 0x00480000;
        uint32 _reserved4; // = 0x00480000;
        uint32 _reserved5; // = 0;
        uint16 _reserved6; // = 1;
        uint8  _reserved7[32]; // = 0;
        uint16 _reserved8; // = 24;
        uint16 _reserved9; // = -1; // (16) SIGNED!

        ESDAtom *_pes;

};

#endif // VISUALSAMPLEENTRY_H_INCLUDED


