


#ifndef DATAINFORMATIONATOM_H_INCLUDED
#define DATAINFORMATIONATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif
#ifndef ATOM_H_INCLUDED
#include "atom.h"
#endif
#ifndef DATAREFERENCEATOM_H_INCLUDED
#include "datareferenceatom.h"
#endif

class DataInformationAtom : public Atom
{

    public:
        DataInformationAtom(MP4_FF_FILE *fp, uint32 size, uint32 type); // Stream-in ctor
        virtual ~DataInformationAtom();

        // Member gets and sets
        const DataReferenceAtom &getDataReferenceAtom() const
        {
            return *_pdataReferenceAtom;
        }
        DataReferenceAtom &getMutableDataReferenceAtom()
        {
            return *_pdataReferenceAtom;
        }

    private:

        DataReferenceAtom *_pdataReferenceAtom;

};

#endif // DATAINFORMATIONATOM_H_INCLUDED

