

#ifndef PV_PLAYER_DATASINK_H_INCLUDED
#define PV_PLAYER_DATASINK_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_STRING_H_INCLUDED
#include "oscl_string.h"
#endif

#ifndef PVMF_FORMAT_TYPE_H_INCLUDED
#include "pvmf_format_type.h"
#endif

class PVMFNodeInterface;

typedef enum
{
    PVP_DATASINKTYPE_UNKNOWN,
    PVP_DATASINKTYPE_FILENAME,
    PVP_DATASINKTYPE_SINKNODE
} PVPDataSinkType;


// CLASS DECLARATION
class PVPlayerDataSink
{
    public:
        /**
         * Object destructor function
         **/
        virtual ~PVPlayerDataSink() {};

        /**
         * Returns this instance's data sink type
         *
         * @returns The PVPlayer data sink type
         **/
        virtual PVPDataSinkType GetDataSinkType() = 0;

        /**
         * Returns this data sink instance's format type
         *
         * @returns The format type
         **/
        virtual PVMFFormatType GetDataSinkFormatType() = 0;

        /**
         * Returns the filename for the data sink
         *
         * @returns The filename as an OSCL_wString
         **/
        virtual OSCL_wString& GetDataSinkFilename() = 0;

        /**
         * Returns the node interface for the data sink
         *
         * @returns The node interface pointer to the data sink
         **/
        virtual PVMFNodeInterface* GetDataSinkNodeInterface() = 0;
};

#endif // PV_PLAYER_DATASINK_H_INCLUDED

