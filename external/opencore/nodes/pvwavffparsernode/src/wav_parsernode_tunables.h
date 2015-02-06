
#ifndef WAV_PARSERNODE_TUNABLES_H_INCLUDED
#define WAV_PARSERNODE_TUNABLES_H_INCLUDED

// Playback clock timescale
#define COMMON_PLAYBACK_CLOCK_TIMESCALE 1000

//Default NODE COMMAND vector reserve size
#define PVMF_WAVFFPARSER_NODE_COMMAND_VECTOR_RESERVE 10

//Starting value for command IDs
#define PVMF_WAVFFPARSER_NODE_COMMAND_ID_START 6000

#define PVWAVFF_MEDIADATA_POOLNUM 10
#define PVWAVFF_MEDIADATA_CHUNKSIZE 128

#define PVWAV_MSEC_PER_BUFFER  100

#define MAX_QUERY_KEY_LEN       256

#define MIN_WAVFFPARSER_RATE 10000
#define MAX_WAVFFPARSER_RATE 500000

#endif /*WAV_PARSERNODE_TUNABLES_H_INCLUDED*/
