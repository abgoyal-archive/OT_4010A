

#ifndef PV_2WAY_BASIC_INTERFACES_H_INCLUDED
#define PV_2WAY_BASIC_INTERFACES_H_INCLUDED


//  INCLUDES

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef PV_UUID_H_INCLUDED
#include "pv_uuid.h"
#endif

#define PV2WayTrackInfoInterfaceUUID PVUuid(0x0864f8a2,0xde16,0x4ced,0xaa,0x48,0xda,0x18,0x20,0xf0,0x75,0xfb) // TBD: generate a new UUID here

class PV2WayTrackInfoInterface : public PVInterface
{
    public:
        /*
           Returns the code and space UUID for this info message

           @param aCode Reference to an integer which will be filled in with event code
           @param aUuid Reference to a PVUuid which will be filled in with UUID assocated to event code

           @return None
        */
        virtual void GetCodeUUID(int32& aCode, PVUuid& aUuid) = 0;

        /*
           Returns a reference to the format string.

           @return Reference to the format string
        */
        virtual void GetFormatString(PVMFFormatType& aMimeString) = 0;

        /*
           Returns the Format Specific Info associated with this track

           @return A pointer to the Format Specific Info.  NULL if FSI is not present
        */
        virtual uint8* GetFormatSpecificInfo(uint32& aFormatSpecificInfoLen) = 0;

        // From PVInterface

        /*
           Increments the reference count for this info message object
         */
        virtual void addRef() = 0;
        /*
           Decrements the reference count for this info message object and deletes
           this object if count goes to 0.
        */
        virtual void removeRef() = 0;
        /*
           Returns the extension interface for the specified UUID if this info
           message object supports it. If the requested extension interface is supported
           true is returned, else false.
        */
        virtual bool queryInterface(const PVUuid& uuid, PVInterface*& iface) = 0;
};


#endif //


