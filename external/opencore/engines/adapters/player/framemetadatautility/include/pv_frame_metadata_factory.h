

#ifndef PV_FRAME_METADATA_FACTORY_H_INCLUDED
#define PV_FRAME_METADATA_FACTORY_H_INCLUDED

// Forward declaration
class PVFrameAndMetadataInterface;
class PVCommandStatusObserver;
class PVInformationalEventObserver;
class PVErrorEventObserver;

class PVFrameAndMetadataFactory
{
    public:

        /**
         * Creates an instance of a pvFrameAndMetadata utility. If the creation fails, this function will leave.
         *
         * @param aOutputFormatMIMEType  The output format when retrieving a frame specified as a MIME string
         * @param aCmdStatusObserver     The observer for command status
         * @param aErrorEventObserver    The observer for unsolicited error events
         * @param aInfoEventObserver     The observer for unsolicited informational events
         *
         * @returns An interface pointer to a pvFrameAndMetadata utility or leaves if instantiation fails
         **/
        OSCL_IMPORT_REF static PVFrameAndMetadataInterface* CreateFrameAndMetadataUtility(char *aOutputFormatMIMEType,
                PVCommandStatusObserver* aCmdStatusObserver,
                PVErrorEventObserver *aErrorEventObserver,
                PVInformationalEventObserver *aInfoEventObserver,
                bool aHwAccelerate = true);
        /**
         * This function allows the application to delete an instance of a pvFrameAndMetadata utility
         * and reclaim all allocated resources.  An utility instance can be deleted only in
         * the idle state. An attempt to delete an instance in any other state will
         * fail and return false.
         *
         * @param aUtility The interface pointer to an utility instance to be deleted.
         *
         * @returns A status code indicating success or failure.
         **/
        OSCL_IMPORT_REF static bool DeleteFrameAndMetadataUtility(PVFrameAndMetadataInterface* aUtility);
};


#endif // PV_FRAME_METADATA_FACTORY_H_INCLUDED


