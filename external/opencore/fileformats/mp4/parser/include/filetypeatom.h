
#ifndef FILETYPEATOM_H_INCLUDED
#define FILETYPEATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef ATOM_H_INCLUDED
#include "atom.h"
#endif

#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif

#define  MINIMUM_SIZE_REQUIRED_TO_READ_MAJOR_BRAND 12
#define  MINIMUM_SIZE_REQUIRED_TO_READ_MINOR_VERSION 16

class FileTypeAtom : public Atom
{

    public:
        OSCL_IMPORT_REF FileTypeAtom(MP4_FF_FILE *fp, uint32 atomSize, uint32 atomType); // stream-in constructor
        OSCL_IMPORT_REF virtual ~FileTypeAtom();

        OSCL_IMPORT_REF uint32 getMajorBrand()
        {
            return _pMajorBrand;
        }
        uint32 getMajorBrandVersion()
        {
            return _pVersion;
        }
        OSCL_IMPORT_REF Oscl_Vector<uint32, OsclMemAllocator> *getCompatibleBrand()
        {
            return _pCompatible_brand;
        }

    private:
        uint32  _pMajorBrand;
        uint32  _pVersion;
        Oscl_Vector<uint32, OsclMemAllocator> *_pCompatible_brand;
};


#endif // FILETYPEATOM_H_INCLUDED

