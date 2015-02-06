
// ----------------------------------------------------------------------
//
// This Software is an original work of authorship of PacketVideo Corporation.
// Portions of the Software were developed in collaboration with NTT  DoCoMo,
// Inc. or were derived from the public domain or materials licensed from
// third parties.  Title and ownership, including all intellectual property
// rights in and to the Software shall remain with PacketVideo Corporation
// and NTT DoCoMo, Inc.
//
// -----------------------------------------------------------------------
/************************************************************************/
/*  file name       :H245Msg.h                  */
/*  file contents   :H245 Message I/F Definition Header     */
/*  draw        :'96.11.15                  */
/*----------------------------------------------------------------------*/
/*  amendment       :                       */
/*      Copyright (C) 1996 NTT DoCoMo               */
/************************************************************************/
#ifndef _H245MSG_H_
#define _H245MSG_H_

#include "oscl_base.h"
/****************************************/
/* H245 Message Interface Structure     */
/****************************************/
typedef struct _h245Msg
{
    uint8   Type1;                  /* Message Type */
    uint8   Type2;                  /* Message Number */
    uint8*  pData;                  /* Real Data Pointer */
} S_H245Msg;
typedef S_H245Msg       *PS_H245Msg;

#endif /* H245MSG */
