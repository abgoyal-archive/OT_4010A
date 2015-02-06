
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
/*  file name       : seclc.h                                           */
/*  file contents   : Close Logical Channel Signalling Entity           */
/*                  :                                 Management Header */
/*  draw            : '96.11.19                                         */
/*----------------------------------------------------------------------*/
/*  amendment       :                                                   */
/*                          Copyright (C) 1996 NTT DoCoMo               */
/************************************************************************/
#ifndef _SECLC_H_
#define _SECLC_H_


/************************************************************************/
/*  Headerfile Include                                                  */
/************************************************************************/
#include    "h245pri.h"
#include    "lcentry.h"
#include    "sebase.h"


/************************************************************************/
/*  Function Prototype Declare                                          */
/************************************************************************/
class CLC : public SEBase
{
    public:
        CLC() : SEBase() {}
        ~CLC() {}

        void Reset() {}

        void _0400_0000(LCEntry*) ;
        void _0401_0001(LCEntry*) ;
        void _0401_0011(LCEntry*) ;
        void _0402_0011(LCEntry*) ;
        void _0403_0011(LCEntry*, PS_RccRejectCause) ;
        void _0404_0010(LCEntry*) ;
        void _0405_0010(LCEntry*, PS_RequestChannelCloseReject) ;
        void _0406_0010(LCEntry*, int32) ;
        void _0407_0011(LCEntry*) ;

    private:
        CLC(const CLC&);

        void MsgCloseSend(LCEntry*) ;
        void MsgCloseAckSend(LCEntry*) ;
        void MsgCloseRjtSend(LCEntry*, PS_RccRejectCause) ;
        void MsgCloseRlsSend(LCEntry*) ;
        void PtvCloseIdcSend(LCEntry*) ;
        void PtvRjtIdcSend(LCEntry*, int32 , PS_RccRejectCause) ;
        void PtvCloseCfmSend(LCEntry*) ;
        int32 T108TimerIdGet(LCEntry*);
        void T108TimerStart(LCEntry*) ;
        void T108TimerStop(LCEntry*) ;
};

#endif /* _SECLC_ */
