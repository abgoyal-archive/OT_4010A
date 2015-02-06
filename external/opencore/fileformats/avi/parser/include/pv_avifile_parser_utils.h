
#ifndef PV_AVIFILE_PARSER_UTILS_H_INCLUDED
#define PV_AVIFILE_PARSER_UTILS_H_INCLUDED

#ifndef PV_AVIFILE_TYPEDEFS_H_INCLUDED
#include "pv_avifile_typedefs.h"
#endif

//utility class
class PVAviFileParserUtils
{

    public:
        static PV_AVI_FILE_PARSER_ERROR_TYPE
        ReadNextChunkType(PVFile* aFp, uint32& aChkType);

        static PV_AVI_FILE_PARSER_ERROR_TYPE
        read32(PVFile* aFp, uint32& aBuff, bool aBigEndian = false);

        static uint32 read8(PVFile* aFp, uint8* aBuff, uint32 aLength);

        static PV_AVI_FILE_PARSER_ERROR_TYPE
        read16(PVFile* aFp, uint16& aBuff, bool aBigEndian = false);

        static int32 GetStreamNumber(uint32 aData);


};


#endif  //#ifndef PV_AVIFILE_PARSER_UTILS_H_INCLUDED

