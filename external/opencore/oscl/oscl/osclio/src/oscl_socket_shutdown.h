

#ifndef OSCL_SOCKET_SHUTDOWN_H_INCLUDED
#define OSCL_SOCKET_SHUTDOWN_H_INCLUDED

#include "oscl_socket_types.h"
#include "oscl_socket_method.h"

class OsclShutdownRequest;

class OsclShutdownMethod : public OsclSocketMethod
{
    public:
        static OsclShutdownMethod *NewL(OsclIPSocketI& c);

        ~OsclShutdownMethod();

        TPVSocketEvent Shutdown(TPVSocketShutdown aHow,
                                int32 aTimeout);

    private:
        OsclShutdownMethod(OsclIPSocketI& c)
                : OsclSocketMethod(c, "osclshutdownmethod", EPVSocketShutdown)
        {}

        void ConstructL();

    public:
        OsclShutdownRequest *ShutdownRequest()
        {
            return (OsclShutdownRequest*)iSocketRequestAO;
        }
};

class OsclShutdownRequest : public OsclSocketRequestAO
{
    public:
        OsclShutdownRequest(OsclSocketMethod& c)
                : OsclSocketRequestAO(c, "osclShutdownRequest")
        {}

        void Shutdown(TPVSocketShutdown aHow);

    private:
        ShutdownParam*Param()
        {
            return (ShutdownParam*)iParam;
        }
};

#endif

