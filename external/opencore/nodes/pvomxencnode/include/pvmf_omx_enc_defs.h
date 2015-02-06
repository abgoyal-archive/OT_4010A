
#ifndef PVMF_OMX_ENC_DEFS_H_INCLUDED
#define PVMF_OMX_ENC_DEFS_H_INCLUDED

// Error and info messages
//
enum PVMFOMXEncNodeErrors
{
    // Enc node failed to initialize the encoder. Fatal error so the node needs to be reset.
    PVOMXENCNODE_ERROR_ENCODER_INIT_FAILED = PVMF_NODE_ERROR_EVENT_LAST
};

enum PVMFOMXEncNodeInfo
{
    // Encoding of a frame failed. Encoder node will continue on to encode the next frame
    PVOMXENCNODE_INFO_ENCODEFRAME_FAILED = PVMF_NODE_INFO_EVENT_LAST,
    // Input bitstream buffer overflowed (frame too large or couldn't find frame marker).
    PVOMXENCNODE_INFO_INPUTBITSTREAMBUFFER_OVERFLOW
};

#endif // PVMF_OMXENC_DEFS_H_INCLUDED


