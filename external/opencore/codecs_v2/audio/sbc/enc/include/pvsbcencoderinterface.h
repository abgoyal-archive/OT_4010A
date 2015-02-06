
#ifndef PVSBCENCODERINTERFACE_H_INCLUDED
#define PVSBCENCODERINTERFACE_H_INCLUDED

// includes
#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_e32std.h"
#endif


#define MAX_SZOF_PCM_BUFF   (512)   /* size in bytes */
#define MAX_SZOF_BS_BUFF    (1036)  /* size in bytes */

typedef enum TPvSbcChanMode
{
    CM_MONO,
    CM_DUAL_CHANNEL,
    CM_STEREO,
    CM_JOINT_STEREO
} TPvSbcChanMode;


enum TPvSbcEncStatus
{
    TPVSBCENC_SUCCESS = 0,
    TPVSBCENC_FAIL = 1,
    TPVSBCENC_INVALID_PARAMS = 2,
    TPVSBCENC_INSUFFICIENT_MEMORY = 3,
    TPVSBCENC_INSUFFICIENT_INPUT_DATA = 4
};


typedef struct  TPvSbcEncConfig
{
    uint    sampling_frequency;
    uint8   nrof_channels;
    uint    channel_mode;
    uint8   block_len;
    uint8   nrof_subbands;
    uint8   bitpool;
    uint    allocation_method;
    uint8   join;
    uint16  *pcmbuffer;
    uint8   *bitstream;
    uint    framelen;
    void    *state;
} TPvSbcEncConfig;


// PVSbcEncoderInterface pure virtual interface class
class PVSbcEncoderInterface
{
    public:
        /*! \fn ~PVSbcEncoderInterface()
         *  \brief A destructor function
         *  \return Void
         */
        OSCL_IMPORT_REF virtual ~PVSbcEncoderInterface() {};

        /*! \fn TPvSbcEncStatus Init()
         *  \brief Used to initialize the encoder.
         *  \return A status code of type TPvSbcEncStatus.
         */
        OSCL_IMPORT_REF virtual TPvSbcEncStatus Init() = 0;

        /*! \fn TPvSbcEncStatus SetInput(TPvSbcEncConfig *aEnc)
         *  \brief Used to send the input parameters to the encoder.
         *  \param aEnc - A variable of structure type TPvSbcEncConfig.
         *  \return A status code of type TPvSbcEncStatus.
         */
        OSCL_IMPORT_REF virtual TPvSbcEncStatus SetInput(TPvSbcEncConfig *aEnc) = 0;

        /*! \fn TPvSbcEncStatus Execute(unsigned short* pcmbuffer, unsigned int, inBufSize, unsigned char *bitstream, unsigned int outBufSize)
         *  \brief This is the main encoding routine of the library.
         *  \param pcmbuffer - Pointer to the pcm buffer to be encoded.
         *  \param bitstream - Pointer to the encoded bitstream.
         *  \return A status code of type TPvSbcEncStatus.
         */
        OSCL_IMPORT_REF virtual TPvSbcEncStatus Execute(uint16* pcmbuffer, uint inBufSize,
                uint8 *bitstream, uint *outBufSize) = 0;

        /*! \fn TPvSbcEncStatus Reset()
         *  \brief Used to reset the encoder.
         *  \return A status code of type TPvSbcEncStatus.
         */
        OSCL_IMPORT_REF virtual TPvSbcEncStatus Reset() = 0;
};

#endif // PVSBCENCODERINTERFACE_H_INCLUDED


