
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                     MPEG-4 InitialObjectDescriptor Class                      */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef INITIALOBJECTDESCRIPTOR_H_INCLUDED
#define INITIALOBJECTDESCRIPTOR_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef OBJECTDESCRIPTOR_H_INCLUDED
#include "objectdescriptor.h"
#endif

#ifndef ES_ID_INC_H_INCLUDED
#include "es_id_inc.h"
#endif

class InitialObjectDescriptor : public ObjectDescriptor
{

    public:
        InitialObjectDescriptor(MP4_FF_FILE *fp); // Stream-in Constructor
        virtual ~InitialObjectDescriptor();

        // Member gets
        bool getIncludeInlineProfilesFlag() const
        {
            return _includeInlineProfilesFlag;
        }
        uint8 getODProfileLevelIndication()
        {
            return _ODProfileLevelIndication;
        }
        uint8 getSceneProfileLevelIndication()
        {
            return _sceneProfileLevelIndication;
        }
        uint8 getAudioProfileLevelIndication()
        {
            return _audioProfileLevelIndication;
        }
        uint8 getVisualProfileLevelIndication()
        {
            return _visualProfileLevelIndication;
        }
        uint8 getGraphicsProfileLevelIndication()
        {
            return _graphicsProfileLevelIndication;
        }
        uint32 getReadCount()
        {
            return _count;
        }

    private:
        // ODID from base clase ObjectDescriptor (10)
        // urlFlag from base class ObjectDescriptor (1)
        bool _includeInlineProfilesFlag; // (1)
        // _reserved from base class - now ONLY (4) instead of (5)

        uint8 _ODProfileLevelIndication; // (8)
        uint8 _sceneProfileLevelIndication; // (8)
        uint8 _audioProfileLevelIndication; // (8)
        uint8 _visualProfileLevelIndication; // (8)
        uint8 _graphicsProfileLevelIndication; // (8)
        uint32 _count;

        Oscl_Vector<ES_ID_Inc *, OsclMemAllocator> * _pES_ID_Inc;
};

#endif // INITIALOBJECTDESCRIPTOR_H_INCLUDED


