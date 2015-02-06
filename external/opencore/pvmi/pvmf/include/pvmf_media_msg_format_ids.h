

#ifndef PVMF_MEDIA_MSG_FORMAT_IDS_H_INCLUDED
#define PVMF_MEDIA_MSG_FORMAT_IDS_H_INCLUDED

#ifndef PV_UUID_H_INCLUDED
#include "pv_uuid.h"
#endif

const PVUid32 PVMF_MEDIA_MSG_FORMAT_UNDEFINED = 0xFFFFFFFF;

/* zero through 255 is reserved for media data */
const PVUid32 PVMF_MEDIA_MSG_DATA_FORMAT_ID = 0x00000000;

/* common commands start from 256 and go upto 511 */
const PVUid32 PVMF_MEDIA_CMD_FORMAT_IDS_START = 0x00000100;

const PVUid32 PVMF_MEDIA_CMD_EOS_FORMAT_ID  = 0x00000101;

const PVUid32 PVMF_MEDIA_CMD_RE_CONFIG_FORMAT_ID = 0x00000102;

/* add new command ids here */
const PVUid32 PVMF_MEDIA_CMD_BOS_FORMAT_ID  = 0x00000103;

/* format id for no-op messages */
const PVUid32 PVMF_MEDIA_CMD_NOOP_FORMAT_ID  = 0x00000104;

const PVUid32 PVMF_MEDIA_CMD_FORMAT_IDS_END = 0x00000200;

/* command ids above 512 are reserved for node specific commands */
#endif

