

/* PASP Box is used to contain the Horizontal and vertical spacing */

#ifndef PASPBOX_H_INCLUDED
#define PASPBOX_H_INCLUDED



#include "atom.h"
#include "atomutils.h"
#include "oscl_file_io.h"

class PASPBox : public Atom
{
    public:
        PASPBox(MP4_FF_FILE *fp, uint32 size, uint32 type);
        virtual ~PASPBox() {};

        uint32 getHspacing()
        {
            return _hSpacing;
        }
        uint32 getVspacing()
        {
            return _vSpacing;
        }


    private:
        uint32 _hSpacing;
        uint32 _vSpacing;

};


#endif //End of PASPBOX_H_INCLUDED

