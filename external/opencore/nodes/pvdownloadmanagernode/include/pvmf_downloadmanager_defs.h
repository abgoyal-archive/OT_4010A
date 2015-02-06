
#ifndef PVMF_DOWNLOADMANAGER_DEFS_H_INCLUDED
#define PVMF_DOWNLOADMANAGER_DEFS_H_INCLUDED


///////////////////////////////////////////////
// Error Events
///////////////////////////////////////////////

//This node uses the extended events reported by the download and parser nodes.
//See pvmfftdmnode_events.h for download node events.
//See pvmf_mp4ffparser_events.h for parser node events.


///////////////////////////////////////////////
// Information Events
///////////////////////////////////////////////

// The download node reports the following PVMF Info events
// in addition to the node-specific events:
//
//  PVMFInfoBufferingStart: Download has started.
//
//  PVMFInfoBufferingStatus: Download progress report in percent complete.
//
//  PVMFInfoBufferingComplete: Download is complete.
//
//  PVMFInfoDataReady: OK to begin playback.
//
//  PVMFInfoEndOfData: End of track detected by parser node.
//


///////////////////////////////////////////////
// Port type tags
///////////////////////////////////////////////

//None.  Port requests must match the format used by the Mp4 Parser node.

#endif // PVMF_DOWNLOADMANAGER_DEFS_H_INCLUDED

