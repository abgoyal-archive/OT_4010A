
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                        MPEG-4 ObjectDescriptor Class                          */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef OBJECTDESCRIPTOR_H_INCLUDED
#define OBJECTDESCRIPTOR_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef BASEDESCRIPTOR_H_INCLUDED
#include "basedescriptor.h"
#endif

#ifndef ESDESCRIPTOR_H_INCLUDED
#include "esdescriptor.h"
#endif

#ifndef ES_ID_REF_H_INCLUDED
#include "es_id_ref.h"
#endif

#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif


class ObjectDescriptor : public BaseDescriptor
{

    public:
        ObjectDescriptor(MP4_FF_FILE *fp, bool all = false); // Stream-in Constructor
        virtual ~ObjectDescriptor();

        // Member gets
        uint16 getObjectDescriptorID() const
        {
            return _objectDescriptorID;
        }
        bool getUrlFlag() const
        {
            return _urlFlag;
        }
        uint8 getUrlLength() const
        {
            return _urlLength;
        }

        // Retuns a UNICODE string given the null-terminated byte array
        OSCL_wHeapString<OsclMemAllocator> getUrlString() const;

    protected:
        uint16 _objectDescriptorID; // (10)
        bool _urlFlag; // (1)
        uint8 _reserved; // 0b11111; // (5)

        uint8 _urlLength; // (8)
        uint8 *_urlStringPtr; // (8)[_urlLength]

        Oscl_Vector<uint32, OsclMemAllocator> *_pESIDVec;
        Oscl_Vector<ES_ID_Ref *, OsclMemAllocator> * _pES_ID_Ref;
};

#endif // OBJECTDESCRIPTOR_H_INCLUDED


