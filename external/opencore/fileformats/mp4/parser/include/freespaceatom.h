
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                           MPEG-4 FreeSpaceAtom Class                          */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef FREESPACEATOM_H_INCLUDED
#define FREESPACEATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef ATOM_H_INCLUDED
#include "atom.h"
#endif

class FreeSpaceAtom : public Atom
{

    public:
        FreeSpaceAtom(); // Constructor
        FreeSpaceAtom(FreeSpaceAtom atom); // Copy Constructor
        virtual ~FreeSpaceAtom();

        // Adding empty data
        void addData(); // Data doesn't matter so no need to pass in any value
        vector<uint8>* getData()
        {
            return _pdata;
        }

        // Rendering the Atom in proper format (bitlengths, etc.) to an ostream
        virtual void renderToFileStream(ofstream &os);
        // Reading in the Atom components from an input stream
        virtual void readFromFileStream(ifstream &is);

    private:
        vector<uint8>* _pdata;
};

#endif // FREESPACEATOM_H_INCLUDED


