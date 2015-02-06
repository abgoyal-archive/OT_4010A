
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
/*****************************************************************************/
/*  file name            : tsc_clc.h                                         */
/*  file contents        :                                                   */
/*  draw                 : '96.10.09                                         */
/*---------------------------------------------------------------------------*/
/*  amendment                                                                */
/*              Copyright (C) 1996 NTT DoCoMo                                */
/*****************************************************************************/

#ifndef TSCCLC_H_INCLUDED
#define TSCCLC_H_INCLUDED

#ifndef PVLOGGER_H_INCLUDED
#include "pvlogger.h"
#endif

#include "h245.h"


class TSC_clc
{
    public:

        TSC_clc() {};
        ~TSC_clc() {};

        void SetH245(H245* aH245)
        {
            iH245 = aH245;
        }

        /* Close Logical Channel */
        void ClcClsReq(uint32);
        void ClcClsRps(uint32);
        void ClcRjtReq(uint32);
    private:
        H245* iH245;
};

#endif

