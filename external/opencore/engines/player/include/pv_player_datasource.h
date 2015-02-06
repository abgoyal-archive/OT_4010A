

#ifndef PV_PLAYER_DATASOURCE_H_INCLUDED
#define PV_PLAYER_DATASOURCE_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_STRING_H_INCLUDED
#include "oscl_string.h"
#endif

#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif

#ifndef PVMF_FORMAT_TYPE_H_INCLUDED
#include "pvmf_format_type.h"
#endif

class PVMFNodeInterface;

typedef enum
{
    PVP_DATASRCTYPE_UNKNOWN,
    PVP_DATASRCTYPE_URL,
    PVP_DATASRCTYPE_SOURCENODE
} PVPDataSourceType;


// CLASS DECLARATION
class PVPlayerDataSource
{
    public:
        /**
         * Object destructor function
         **/
        virtual ~PVPlayerDataSource() {};

        /**
         * Returns this instance's data source type
         *
         * @returns The PVPlayer data source type
         **/
        virtual PVPDataSourceType GetDataSourceType() = 0;

        /**
         * Returns this data source instance's format type
         *
         * @returns The format type
         **/
        virtual PVMFFormatType GetDataSourceFormatType() = 0;

        /**
         * Returns the URL for the data source
         *
         * @returns The URL as an OSCL_wString
         **/
        virtual OSCL_wString& GetDataSourceURL() = 0;

        /**
         * Returns the opaque data specific for the data source
         *
         * @returns The opaque data as OsclAny pointer
         **/
        virtual OsclAny* GetDataSourceContextData() = 0;

        /**
         * Returns the node interface for the data source
         *
         * @returns The node interface pointer to the data source
         **/
        virtual PVMFNodeInterface* GetDataSourceNodeInterface() = 0;

        /**
         * Sets alternate source format type. The format types
         * would internally go into a queue(FIFO). So in case the caller
         * has a certain order of priority for alternates, then the
         * caller would have to make sure the calls are made in that
         * order.
         * These alternates are used in case the app wants to the
         * engine to try alternate source nodes for the same url
         *
         * @returns true if the alternate source format type was
         * successfully added to the queue, false otherwise.
         **/
        virtual bool SetAlternateSourceFormatType(PVMFFormatType aFormatType) = 0;

        /**
         * Returns the number of alternate source format types, if any.
         * These alternates are used in case the app wants to the
         * engine to try alternate source nodes for the same url
         *
         * @returns Number of alternate source format types
         **/
        virtual uint32 GetNumAlternateSourceFormatTypes() = 0;

        /**
         * Returns the alternate source format type, based on index.
         * (Index is zero based)
         * These alternates are used in case the app wants to the
         * engine to try alternate source nodes for the same url
         *
         * @returns true if a valid source format type is found
         * corresponding to the mentioned index, false otherwise.
         **/
        virtual bool GetAlternateSourceFormatType(PVMFFormatType& aFormatType,
                uint32 aIndex) = 0;

};

#endif // PV_PLAYER_DATASOURCE_H_INCLUDED

