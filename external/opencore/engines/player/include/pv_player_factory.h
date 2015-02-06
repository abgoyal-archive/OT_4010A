

#ifndef PV_PLAYER_FACTORY_H_INCLUDED
#define PV_PLAYER_FACTORY_H_INCLUDED

// Forward declaration
class PVPlayerInterface;
class PVCommandStatusObserver;
class PVInformationalEventObserver;
class PVErrorEventObserver;
class PVPlayerDataSink;
class PvmiMIOControl;

class PVPlayerFactory
{
    public:

        /**
         * Creates an instance of a pvPlayer engine. If the creation fails, this function will leave.
         *
         * @param aCmdStatusObserver     The observer for command status
         * @param aErrorEventObserver    The observer for unsolicited error events
         * @param aInfoEventObserver     The observer for unsolicited informational events
         *
         * @returns A pointer to a player or leaves if instantiation fails
         **/
        OSCL_IMPORT_REF static PVPlayerInterface* CreatePlayer(PVCommandStatusObserver* aCmdStatusObserver,
                PVErrorEventObserver *aErrorEventObserver,
                PVInformationalEventObserver *aInfoEventObserver,
                uint32* pUserData,
                bool aHwAccelerated = true);
        /**
         * This function allows the application to delete an instance of a pvPlayer
         * and reclaim all allocated resources.  A player can be deleted only in
         * the idle state. An attempt to delete a player in any other state will
         * fail and return false.
         *
         * @param aPlayer The player to be deleted.
         *
         * @returns A status code indicating success or failure.
         **/
        OSCL_IMPORT_REF static bool DeletePlayer(PVPlayerInterface* aPlayer);
};



class PVPlayerDataSinkFactory
{
    public:
        /**
         * Creates an instance of a pvPlayer engine. If the creation fails, this function will leave.
         *
         * @param io_interface_ptr   A pointer to a media io interface
         *
         * @returns A pointer to a player data sink or leaves if instantiation fails
         **/

        OSCL_IMPORT_REF static PVPlayerDataSink* CreateDataSink(PvmiMIOControl* io_interface_ptr);

        /**
         * This function allows the application to delete an instance of the player data sink
         * and reclaim all allocated resources.
         *
         * @param data_sink_ptr The pointer to the player data sink to be deleted
         *
         * @returns A status code indicating success or failure.
         **/

        OSCL_IMPORT_REF static bool DeletePVPlayerSink(PVPlayerDataSink* data_sink_ptr);
};


#endif // PV_PLAYER_FACTORY_H_INCLUDED
