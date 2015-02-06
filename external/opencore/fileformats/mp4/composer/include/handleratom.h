


#ifndef __HandlerAtom_H__
#define __HandlerAtom_H__

#include "fullatom.h"

class PVA_FF_HandlerAtom : public PVA_FF_FullAtom
{

    public:
        PVA_FF_HandlerAtom(int32 handlerType, uint8 version, uint32 flags); // Constructor
        virtual ~PVA_FF_HandlerAtom();

        // Private member gets and sets
        uint32 getHandlerType() const
        {
            return _handlerType;
        }
        void setHandlerType(uint32 type)
        {
            _handlerType = type;
        }

        PVA_FF_UTF8_STRING_PARAM getName() const
        {
            return _name;
        }
        void setName(PVA_FF_UTF8_STRING_PARAM name)
        {
            _name = name;
        }

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

    private:
        void init(int32 handlerType);
        virtual void recomputeSize();

        uint32 reserved1;
        uint32 _handlerType;
        PVA_FF_UTF8_HEAP_STRING _name;
};


#endif

