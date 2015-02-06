
#ifndef PV_PLAYER_ENGINE_TUNABLES_H_INCLUDED
#define PV_PLAYER_ENGINE_TUNABLES_H_INCLUDED


// Always start from the requested begin position when repositioning
#define PVPLAYERENGINE_CONFIG_SKIPTOREQUESTEDPOS_DEF true

// Synchronization margin for media tracks in milliseconds
// Default
#define PVPLAYERENGINE_CONFIG_SYNCMARGIN_EARLY_DEF -20
#define PVPLAYERENGINE_CONFIG_SYNCMARGIN_LATE_DEF 50

// Platforms that need the video decoder node priority to be lower than the
// rest of the nodes need to set the following to 1
#define VIDEO_DEC_NODE_LOW_PRIORITY 1

#endif // PV_PLAYER_ENGINE_TUNABLES_H_INCLUDED

