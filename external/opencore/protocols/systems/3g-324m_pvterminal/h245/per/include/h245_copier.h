
// ===================================================================
// FILE: h245_copier.h
//
// DESC: PER Copy routines for H.245
// -------------------------------------------------------------------
//  Copyright (c) 1998- 2000, PacketVideo Corporation.
//                   All Rights Reserved.
// ===================================================================

// ****************************************************
// ** NOTE: This file is not auto-generated.         **
// ** Add copy routines as needed in the application **
// ****************************************************

#ifndef PER_COPIER
#define PER_COPIER
#ifndef OSCL_TYPES_H_INCLUDED
#include "oscl_types.h"
#endif
OSCL_IMPORT_REF PS_DataType Copy_DataType(PS_DataType x);
OSCL_IMPORT_REF PS_H223LogicalChannelParameters Copy_H223LogicalChannelParameters(PS_H223LogicalChannelParameters x);
PS_MultiplexEntryDescriptor Copy_MultiplexEntryDescriptor(PS_MultiplexEntryDescriptor x);
PS_ForwardLogicalChannelParameters Copy_ForwardLogicalChannelParameters(PS_ForwardLogicalChannelParameters x);
PS_ReverseLogicalChannelParameters Copy_ReverseLogicalChannelParameters(PS_ReverseLogicalChannelParameters x);
PS_TerminalCapabilitySet Copy_TerminalCapabilitySet(PS_TerminalCapabilitySet x);
#endif
