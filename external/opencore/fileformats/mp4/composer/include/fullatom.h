


#ifndef __FullAtom_H__
#define __FullAtom_H__

#include "atom.h"


class PVA_FF_FullAtom : public PVA_FF_Atom
{

    public:
        PVA_FF_FullAtom(uint32 type, uint8 version, uint32 flags); // Constructor
        virtual ~PVA_FF_FullAtom();

        // No "set" methods as they get set directly in the constructor
        uint8 getVersion() const
        {
            return _version;
        }
        uint32 getFlags() const
        {
            return _flags;
        }

        virtual uint32 getDefaultSize() const;

        // Render the base members of PVA_FF_Atom - i.e. the type and size
        virtual bool renderAtomBaseMembers(MP4_AUTHOR_FF_FILE_IO_WRAP *fp) const;
        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp) = 0;

    private:
        uint8 _version; // 1 (8bits)
        uint32 _flags; // 3 (24bits) -- Will need to crop when writing to stream
};


#endif

