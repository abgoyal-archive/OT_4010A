


#ifndef __Atom_H__
#define __Atom_H__

#ifndef OSCL_TYPES_H_INCLUDED
#include "oscl_types.h"
#endif

#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif

#ifndef PARENTABLE_H_INCLUDED
#include "parentable.h"
#endif

#ifndef RENDERABLE_H_INCLUDED
#include "renderable.h"
#endif

const uint32 DEFAULT_ATOM_SIZE = 8; //(8 bytes - 64 bits: 4 bytes for type, 4 for size)

class PVA_FF_Atom : public PVA_FF_Parentable, public PVA_FF_Renderable
{

    public:
        PVA_FF_Atom(uint32 type); // Constructor
        virtual ~PVA_FF_Atom();

        // Member get methods
        virtual uint32 getSize() const
        {
            return _size;
        }
        uint32 getType() const
        {
            return _type;
        }

        uint64 getLargeSize() const
        {
            return _largeSize;
        }
        uint8 getUserType() const
        {
            return _userType;
        }

        // Overriden by all derived classes.  Called when change to atom that affects its _size takes place
        virtual void recomputeSize() = 0; // To implement the PVA_FF_Parentable interface
        virtual uint32 getDefaultSize() const;

        // Render the base members of PVA_FF_Atom - i.e. the type and size
        virtual bool renderAtomBaseMembers(MP4_AUTHOR_FF_FILE_IO_WRAP *fp) const;
        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ofstream
        // Each subclass will override this method to render its own contents.
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp) = 0;

    protected:
        uint32 _size; // 4 (32bits)
        uint32 _type; // 4 (32bits)

    private:
        uint64 _largeSize; // 8 (64bits)
        uint8  _userType; // 1 (8bits)


};



#endif

