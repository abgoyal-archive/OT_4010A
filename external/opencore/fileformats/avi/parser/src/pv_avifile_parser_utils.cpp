
#ifndef PV_AVIFILE_STATUS
#include "pv_avifile_status.h"
#endif

#ifndef PV_AVIFILE_PARSER_UTILS_H_INCLUDED
#include "pv_avifile_parser_utils.h"
#endif



PV_AVI_FILE_PARSER_ERROR_TYPE
PVAviFileParserUtils::ReadNextChunkType(PVFile* aFp, uint32& aChkType)
{
    uint32 data = 0;

    if (PV_AVI_FILE_PARSER_SUCCESS != read32(aFp, data))
    {
        return PV_AVI_FILE_PARSER_READ_ERROR;
    }

    aChkType = data;
    PV_AVI_FILE_PARSER_ERROR_TYPE error = PV_AVI_FILE_PARSER_ERROR_UNKNOWN;

    if (aChkType == RIFF
            || aChkType == AVI
            || aChkType == LIST
            || aChkType == HDRL
            || aChkType == STRL
            || aChkType == STRH
            || aChkType == STRF
            || aChkType == STRN
            || aChkType == STRD
            || aChkType == IDX1
            || aChkType == VIDS
            || aChkType == AUDS
            || aChkType == MIDS
            || aChkType == TXTS
            || aChkType == DIB
            || aChkType == AVIH
            || aChkType == MOVI
            || aChkType == JUNK)
    {
        error = PV_AVI_FILE_PARSER_SUCCESS ;
    }
    else
    {
        error = PV_AVI_FILE_PARSER_UNSUPPORTED_CHUNK;
    }

    return error;
}

PV_AVI_FILE_PARSER_ERROR_TYPE
PVAviFileParserUtils::read32(PVFile* aFp, uint32& aBuff, bool aBigEndian)
{
    aBuff = 0;
    const uint32 sz = 4;
    uint8  temp[sz] = {0};

    if (aFp->Read(temp, 1, sz) != sz)
    {
        return PV_AVI_FILE_PARSER_ERROR_UNKNOWN;
    }

    if (aBigEndian)
    {
        big_endian_to_host((char*)temp, sz);
    }

    for (uint32 ii = 0; ii < sz; ii++)
    {
        aBuff = (aBuff << 8 | temp[ii]);
    }

    return PV_AVI_FILE_PARSER_SUCCESS;
}

uint32 PVAviFileParserUtils::read8(PVFile* aFp, uint8* aBuff, uint32 aLength)
{
    for (uint32 ii = 0; ii < aLength; ii++)
    {
        uint8 temp = 0;
        if (aFp->Read(&temp, 1, 1) != 1)
        {
            return 0;
        }
        else
        {
            aBuff[ii] = temp;
        }
    }
    return aLength;
}

PV_AVI_FILE_PARSER_ERROR_TYPE
PVAviFileParserUtils::read16(PVFile* aFp, uint16& aBuff, bool aBigEndian)
{
    aBuff = 0;
    const uint32 sz = 2;
    uint8  temp[sz] = {0};

    if (aFp->Read(temp, 1, sz) != sz)
    {
        return PV_AVI_FILE_PARSER_ERROR_UNKNOWN;
    }

    if (aBigEndian)
    {
        big_endian_to_host((char*)temp, sz);
    }

    for (uint32 ii = 0; ii < sz; ii++)
    {
        aBuff = (aBuff << 8 | temp[ii]);
    }
    return PV_AVI_FILE_PARSER_SUCCESS;
}

int32 PVAviFileParserUtils::GetStreamNumber(uint32 aData)
{
    uint8 temp[3] = {0};
    temp[0] = aData >> 24;
    temp[1] = ((aData >> 16) & 0xff);
    temp[2] = '\0';

    uint32 strNum = 0;
    if (!PV_atoi((const char*)temp, 'd', 2, strNum))
    {
        return -1;
    }
    else
    {
        return strNum;
    }
}


