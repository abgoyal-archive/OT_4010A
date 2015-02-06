
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                               Parentable Class                                */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef PARENTABLE_H_INCLUDED
#define PARENTABLE_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

class Parentable
{

    public:
        void setParent(Parentable *parent)
        {
            _pparent = parent;
        }
        Parentable *getParent()
        {
            return _pparent;
        }

    protected:
        Parentable *_pparent;
};

#endif // PARENTABLE_H_INCLUDED


