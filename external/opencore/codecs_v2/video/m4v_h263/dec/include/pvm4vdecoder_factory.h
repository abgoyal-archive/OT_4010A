
#ifndef PVM4VDECODER_FACTORY_H_INCLUDED
#define PVM4VDECODER_FACTORY_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

class PVVideoDecoderInterface;

class PVM4VDecoderFactory
{
    public:
        /**
         * Creates an instance of a PVM4VDecoder. If the creation fails, this function will leave.
         *
         * @returns A pointer to an instance of PVM4VDecoder as PVVideoDecoderInterface reference or leaves if instantiation fails
         **/
        OSCL_IMPORT_REF static PVVideoDecoderInterface* CreatePVM4VDecoder(void);

        /**
         * Deletes an instance of PVM4VDecoder and reclaims all allocated resources.
         *
         * @param aVideoDec The PVM4VDecoder instance to be deleted
         * @returns A status code indicating success or failure of deletion
         **/
        OSCL_IMPORT_REF static bool DeletePVM4VDecoder(PVVideoDecoderInterface* aVideoDec);
};

#endif

