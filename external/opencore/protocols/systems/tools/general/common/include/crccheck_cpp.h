

#ifndef _CRCCHECK_CPP_H
#define _CRCCHECK_CPP_H

#include "oscl_base.h"
#include "media_packet.h"
#include "pvmf_media_data_impl.h"
#define NO_ERR      0
#define CRC_ERR     1
#define SN_ERR      2

class CRC
{
    public:
        OSCL_IMPORT_REF CRC();
        ~CRC() {};
        OSCL_IMPORT_REF uint8 Crc8Check(uint8 *pData, int16 Size);
        OSCL_IMPORT_REF uint8 Crc8Check(Packet *pPkt);
        OSCL_IMPORT_REF uint8 Crc8Check(OsclSharedPtr<PVMFMediaDataImpl>& mediaData, bool hasCRC = false);

        OSCL_IMPORT_REF uint16 Crc16Check(uint8 *pData, int16 Size);
        OSCL_IMPORT_REF uint16 Crc16Check(Packet *pPkt);
        OSCL_IMPORT_REF uint16 Crc16Check(OsclSharedPtr<PVMFMediaDataImpl>& mediaData, bool hasCRC = false);

        uint16  CrcTbl8[256];
        uint16  CrcTbl16[256];
};

#endif  /* _CRCCHECK_H */
