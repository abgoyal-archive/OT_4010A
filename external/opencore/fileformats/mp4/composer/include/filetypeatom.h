

#ifndef __FileTypeAtom_H__
#define __FileTypeAtom_H__

#include "atom.h"
#include "atomutils.h"

typedef Oscl_Vector<uint32, OsclMemAllocator> uint32VecType;

class PVA_FF_FileTypeAtom : public PVA_FF_Atom
{

    public:
        PVA_FF_FileTypeAtom(); // Constructor

        virtual ~PVA_FF_FileTypeAtom()
        {
            if (_pCompatibleBrandVec != NULL)
            {
                PV_MP4_FF_TEMPLATED_DELETE(NULL, uint32VecType, Oscl_Vector, _pCompatibleBrandVec);
            }
        }

        void setMajorBrand(uint32 brand)
        {
            _Brand = brand;
        }
        void setMajorBrandVersion(uint32 version)
        {
            _Version = version;
        }
        void addCompatibleBrand(uint32 brand);

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

        virtual void recomputeSize();

    protected:
        uint32 _Brand;
        uint32 _Version;

        Oscl_Vector<uint32, OsclMemAllocator>* _pCompatibleBrandVec;
};

#endif
