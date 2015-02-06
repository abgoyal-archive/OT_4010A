
#ifndef PVAUTHORENGINEFACTORY_H_INCLUDED
#define PVAUTHORENGINEFACTORY_H_INCLUDED

// Forward declaration
class PVAuthorEngineInterface;
class PVCommandStatusObserver;
class PVInformationalEventObserver;
class PVErrorEventObserver;


class PVAuthorEngineFactory
{
    public:
        /**
         * Creates an instance of a pvAuthor engine. If the creation fails, this function will leave.
         *
         * @param aCmdStatusObserver     The observer for command status
         * @param aErrorEventObserver    The observer for unsolicited error events
         * @param aInfoEventObserver     The observer for unsolicited informational events
         * @returns A pointer to an author or leaves if instantiation fails
         **/
        OSCL_IMPORT_REF static PVAuthorEngineInterface* CreateAuthor(PVCommandStatusObserver* aCmdStatusObserver,
                PVErrorEventObserver *aErrorEventObserver, PVInformationalEventObserver *aInfoEventObserver);

        /**
         * This function allows the application to delete an instance of a pvAuthor
         * and reclaim all allocated resources.  An author can be deleted only in
         * the idle state. An attempt to delete an author in any other state will
         * fail and return false.
         *
         * @param aAuthor The author to be deleted.
         * @returns A status code indicating success or failure.
         **/
        OSCL_IMPORT_REF static bool DeleteAuthor(PVAuthorEngineInterface* aAuthor);
};

#endif // PVAUTHORENGINEFACTORY_H_INCLUDED
