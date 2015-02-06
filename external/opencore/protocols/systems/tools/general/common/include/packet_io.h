
// -*- c++ -*-
#ifndef PACKET_IO_H
#define PACKET_IO_H

#include "packet.h"

typedef enum
{
    PACKET_IO_SUCCESS = 0,
    PACKET_IO_FAILURE
} TPvPacketIoStatus;

class PacketInputObserver
{
    public:
        virtual ~PacketInputObserver() {}
        virtual void PacketInComplete(TPvPacketIoStatus status) = 0;
};

/* A PacketInput interface is implemented to receive packets.  */
class PacketInput
{
    public:
        PacketInput() : iPacketInputObserver(NULL)
        {

        }
        virtual void PacketIn(Packet* pack) = 0;
        virtual void PacketIn(Packet* pack, bool /*crc_error*/, int32 /*seq_num_error*/)
        {
            PacketIn(pack);
        }
        virtual ~PacketInput() {}
        virtual void SetPacketInputObserver(PacketInputObserver* observer)
        {
            iPacketInputObserver = observer;
        }
    protected:
        PacketInputObserver* iPacketInputObserver;
};

/* PacketOutput interface is implemented to indicate packets via the PacketInput interface */
class PacketOutput
{
    public:
        PacketOutput() : pPktOutput(NULL)
        {
        }
        void SetPacketOutput(PacketInput* packet_output)
        {
            pPktOutput = packet_output;
        }
        void ResetPacketOutput(void)
        {
            pPktOutput = NULL;
        }
    protected:
        PacketInput* pPktOutput;
};

#endif
