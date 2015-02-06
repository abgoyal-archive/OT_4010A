

#ifndef __DecoderSpecificInfo_H__
#define __DecoderSpecificInfo_H__

#include "basedescriptor.h"
#include "textsampledescinfo.h"

class PVA_FF_DecoderSpecificInfo : public PVA_FF_BaseDescriptor
{

    public:
        PVA_FF_DecoderSpecificInfo(); // Default constructor

        PVA_FF_DecoderSpecificInfo(uint8 *pdata, uint32 size); // Constructor
        PVA_FF_DecoderSpecificInfo(PVA_FF_TextSampleDescInfo *pdata, uint32 size); //Constructor for timed text case

        virtual ~PVA_FF_DecoderSpecificInfo(); // Destructor

        void addInfo(uint8 *info, uint32 size);

        // Member get methods
        uint32 getInfoSize() const
        {
            return _infoSize;    // Returns the size of the info data
        }
        uint8  *getInfo() const
        {
            return _pinfo;    // Returns the byte pointer to info
        }

        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);
        virtual void recomputeSize();

    private:
        uint32 _infoSize;
        uint8  *_pinfo;
};

#endif

