
#ifndef CONTROL_MSG_HDR_H
#define CONTROL_MSG_HDR_H

#include "oscl_base.h"

/************************************************************************/
/*  Interface Information Structure Type Definitions                    */
/************************************************************************/
typedef struct _control_msg_header
{
    enum TDirection
    {
        INCOMING = 1,
        OUTGOING = 2
    };

    uint32    InfType ;               /* Information Type */
    uint32    InfId ;                 /* Information Id */
    /* User Operation Id */
    /* Primitive Id */
    /* Internal Error Occurence Routine */
    /* Timeout Id */
    uint32    InfSupplement1 ;        /* Information Supplement 1 */
    uint32    InfSupplement2 ;        /* Information Supplement 2 */
    uint8*    pParameter ;            /* Primitive Information Pointer ( cf.infptv.h ) */
    uint32    Size ;                  /* Information Parameter Size */
    TDirection Dir;                 /* Incoming/Outgoing SE */

    uint8*    EncodedMsg;           /* Per encoded version of this message */
    uint32    EncodedMsgSize;

} S_ControlMsgHeader ;
typedef S_ControlMsgHeader *PS_ControlMsgHeader ;


#endif
