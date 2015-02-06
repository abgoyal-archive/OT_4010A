
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
/*  file name       : selcblccmn.h                                      */
/*  file contents   : Uni-Directional and Bi-Directional                */
/*                  :              Logical Channel Signalling Entity    */
/*                  :                          Common Management Header */
/*  draw            : '96.11.11                                         */
/*----------------------------------------------------------------------*/
/*  amendment       :                                                   */
/*                          Copyright (C) 1996 NTT DoCoMo               */
/************************************************************************/
#ifndef _SELCBLCCMN_H_
#define _SELCBLCCMN_H_

#include "lcentry.h"
#include "sebase.h"

/************************************************************************/
/*  Class Declaration                                                   */
/************************************************************************/
class LCBLCCmn : public SEBase
{
    public:
        LCBLCCmn() : SEBase() {}
        ~LCBLCCmn() {}

        int32 T103TimerIdGet(uint32 forwardLcn) ;
        void T103TimerStart(uint32 forwardLcn, uint32 tmrSqcNumber) ;
        void T103TimerStop(uint32 forwardLcn) ;

        void MsgOpenSend(uint16 forwardLcn, PS_ForwardReverseParam) ;
        void MsgOpenAckSend(uint16 forwardLcn, PS_ReverseData) ;
        void MsgOpenCfmSend(uint16 forwardLcn, PS_OpenLogicalChannelAck) ;
        void MsgOpenRjtSend(uint16 forwardLcn, PS_OlcRejectCause) ;
        void MsgCloseSend(uint16 forwardLcn, PS_Source) ;
        void MsgCloseAckSend(uint16 forwardLcn) ;

        void PtvEtbIdcSend(uint32 forwardLcn, PS_OpenLogicalChannel, int32 reverseLcn = -1) ;
        void PtvEtbCfmSend(uint32 forwardLcn, PS_OpenLogicalChannelAck, int32 reverseLcn = -1) ;

        void PtvEtbCfmSend2(uint32 forwardLcn, PS_OpenLogicalChannelAck, int32 reverseLcn = -1) ;
        void PtvRlsIdcSend(uint32 forwardLcn, PS_Source , PS_OlcRejectCause, S_InfHeader::TDirection dir, int32 reverseLcn = -1) ;
        void PtvRlsCfmSend(uint32 forwardLcn, int32 reverseLcn = -1) ;
        void PtvErrIdcSend(uint32 forwardLcn, ENUM_ErrCode, int32 reverseLcn = -1) ;

    private:
        LCBLCCmn(const LCBLCCmn&);
};

#endif /* _SELCBLCCMN */
