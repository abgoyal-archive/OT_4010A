
#ifndef FONTRECORD_H_INCLUDED
#define FONTRECORD_H_INCLUDED




#include "atom.h"
#include "atomutils.h"
#include "oscl_file_io.h"

//This class is specific to the Timed text file format
//It holds the information of related to the font i.e fontid, fontlength,
//fontname which will be rendered to the screen.
class PVA_FF_FontRecord : public PVA_FF_Atom
{

    public:
        PVA_FF_FontRecord(uint16 FontListID, uint16 FontID, int8 FontLength, uint8* FontName);
        virtual ~PVA_FF_FontRecord();  // Destructor
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);
        virtual void recomputeSize();
        virtual uint32 getSize();

    private:
        uint16 _fontID;
        int8   _fontLength;
        uint8  *_pFontName;

};

#endif // FONTRECORD_H_INCLUDED
