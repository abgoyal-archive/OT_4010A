
#ifndef PV_2WAY_H324M_TYPES_H_INCLUDED
#define PV_2WAY_H324M_TYPES_H_INCLUDED

#ifndef PV_2WAY_BASIC_TYPES_H_INCLUDED
#include "pv_2way_basic_types.h"
#endif

// MACROS
/** The maximum number of supported formats for user input **/
#define PV_2WAY_MAX_USER_INPUT_FORMATS 4
/** The maximum skew that can be taken into account for both outgoing and incoming sides **/
#define PV_2WAY_MAX_SKEW_MS 1000

typedef enum TPVPostDisconnectOption
{
    EDisconnectLine,
    EAnalogueTelephony
} PV2WayPostDisconnectOption;


typedef enum TPVUserInputType
{
    EAlphanumeric = 0,
    EDtmf,
} PV2WayUserInputType;


class PV2Way324InitInfo : public PV2WayInitInfo
{
    public:
        /**
         * Retrieves the class name
         *
         * @param aClassName
         *         A reference to an OSCL_wString, which is to hold the subclass
         *          name, this class will assign the string "CPV2Way324InitInfo"
         * @returns void
         **/
        virtual void GetInitInfoClassName(OSCL_wString &aClassName)
        {
            aClassName = _STRLIT_WCHAR("CPV2Way324InitInfo");
        }
        PV2Way324InitInfo() : PV2WayInitInfo(), iMultiplexingDelayMs(0) {};
        virtual ~PV2Way324InitInfo() {};

        /**
        *  The Multiplexing delay in Milliseconds
        **/
        uint16 iMultiplexingDelayMs;
};


class PV2Way324ConnectOptions : public PV2WayConnectOptions
{
    public:
        /**
         * Constructor
         * @param disconnectTimeout
         *         The interval to wait after initiating a disconnect before stopping signalling
         *
         **/
        PV2Way324ConnectOptions(uint32 aDisconnectTimeoutInterval)
                : iDisconnectTimeoutInterval(aDisconnectTimeoutInterval) {}

        PV2Way324ConnectOptions() : PV2WayConnectOptions(), iDisconnectTimeoutInterval(0) {};
        virtual ~PV2Way324ConnectOptions() {};

        /**
         * Retrieves the class name
         *
         * @param aClassName
         *         A reference to an OSCL_wString, which is to hold the subclass
         *          name, this class will assign the string "CPV2Way324ConnectInfo"
         * @returns void
         **/
        virtual void GetConnectInfoClassName(OSCL_wString &aClassName)
        {
            aClassName = _STRLIT_WCHAR("PV2Way324ConnectOptions");
        }

        /**
        * The disconnect timeout interval in units of 100ms
        **/
        uint32 iDisconnectTimeoutInterval;
};

class PVH223AlConfig
{
    public:
        enum PVH223AlIndex
        {
            PVH223_AL1 = 1,
            PVH223_AL2 = 2,
            PVH223_AL3 = 4
        };
        virtual PVH223AlIndex IsA() const = 0;
};


class PVH223Al1Config : public PVH223AlConfig
{
    public:
        PVH223AlIndex IsA()const
        {
            return PVH223_AL1;
        }

        bool iFramed;
};

class PVH223Al2Config : public PVH223AlConfig
{
    public:
        PVH223AlIndex IsA()const
        {
            return PVH223_AL2;
        }

        bool iUseSequenceNumbers;
};

class PVH223Al3Config : public PVH223AlConfig
{
    public:
        PVH223AlIndex IsA()const
        {
            return PVH223_AL3;
        }

        uint32 iControlFieldOctets;
        uint32 iSendBufferSize;
};
#endif
