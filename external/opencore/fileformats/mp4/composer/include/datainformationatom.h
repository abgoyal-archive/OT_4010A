


#ifndef __DataInformationAtom_H__
#define __DataInformationAtom_H__

#include "atom.h"
#include "datareferenceatom.h"

class PVA_FF_DataInformationAtom : public PVA_FF_Atom
{

    public:
        PVA_FF_DataInformationAtom(); // Constructor

        virtual ~PVA_FF_DataInformationAtom();

        // Member gets and sets
        const PVA_FF_DataReferenceAtom &getDataReferenceAtom() const
        {
            return *_pdataReferenceAtom;
        }
        PVA_FF_DataReferenceAtom &getMutableDataReferenceAtom()
        {
            return *_pdataReferenceAtom;
        }
        void setDataReferenceAtom(PVA_FF_DataReferenceAtom *dra)
        {
            _pdataReferenceAtom = dra;
        }

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

    private:
        virtual void recomputeSize();

        PVA_FF_DataReferenceAtom *_pdataReferenceAtom;

};



#endif

