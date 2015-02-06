


#ifndef __SampleEntry_H__
#define __SampleEntry_H__

#include "fullatom.h"
#include "esdescriptor.h"


class PVA_FF_SampleEntry : public PVA_FF_Atom
{

    public:
        PVA_FF_SampleEntry(uint32 format); // Constructor
        virtual ~PVA_FF_SampleEntry();

        // Member gets and sets
        uint16 getDataReferenceIndex() const
        {
            return _dataReferenceIndex;
        }
        void setDataReferenceIndex(uint16 index)
        {
            _dataReferenceIndex = index;
        }

        // Getting and setting the Mpeg4 VOL header
        virtual PVA_FF_DecoderSpecificInfo *getDecoderSpecificInfo() const
        {
            return NULL;
        }
        virtual void addDecoderSpecificInfo(PVA_FF_DecoderSpecificInfo *pinfo)
        {
            OSCL_UNUSED_ARG(pinfo);
        };

        //call only in the case of timed text
        virtual void addTextDecoderSpecificInfo(PVA_FF_TextSampleDescInfo *pinfo)
        {
            OSCL_UNUSED_ARG(pinfo);
        };

        virtual void setESID(uint16 esid)
        {
            OSCL_UNUSED_ARG(esid);
        };

    protected:
        // Reserved constants
        uint8 _reserved[6];
        uint16 _dataReferenceIndex;

};


#endif

