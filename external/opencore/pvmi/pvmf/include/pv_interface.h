

#ifndef PV_INTERFACE_H
#define PV_INTERFACE_H


#ifndef PV_UUID_H_INCLUDED
#include "pv_uuid.h"
#endif

class PVInterface
{

    public:
        virtual void addRef() = 0;
        virtual void removeRef() = 0;
        virtual bool queryInterface(const PVUuid& uuid, PVInterface*& iface) = 0;

        virtual ~PVInterface() {}
};

#endif
