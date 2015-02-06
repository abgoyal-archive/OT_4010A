
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

/*  file name            : tsc_eventreceive.h                                */

/*  file contents        : Event Id Receive Header                           */

/*  draw                 : '96.10.04                                         */

/*---------------------------------------------------------------------------*/

/*  amendment                                                                */

/*              Copyright (C) 1996 NTT DoCoMo                                */

/*****************************************************************************/

#if !defined (_TSC_EVENTRECEIVE)

#define _TSC_EVENTRECEIVE
#include "pvt_params.h"


/*---------------------------------------------------------------------------*/

/*  Prototype Declare                                                        */

/*---------------------------------------------------------------------------*/

uint32 Tsc_EventReceive(PS_ControlMsgHeader pReceiveInf);

uint32 Tsc_UserEventLookup(PS_ControlMsgHeader pReceiveInf);



#endif

