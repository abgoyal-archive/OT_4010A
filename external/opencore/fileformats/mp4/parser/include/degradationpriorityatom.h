
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                     MPEG-4 DegradationPriorityAtom Class                      */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef DEGRADATIONPRIORITYATOM_H_INCLUDED
#define DEGRADATIONPRIORITYATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef FULLATOM_H_INCLUDED
#include "fullatom.h"
#endif

class DegradationPriorityAtom : public FullAtom
{

    public:
        DegradationPriorityAtom(uint8 version, uint32 flags); // Constructor
        DegradationPriorityAtom(DegradationPriorityAtom atom); // Copy Constructor
        virtual ~DegradationPriorityAtom();

        // Adding to and getting the sample priority values
        void addPriority(uint16 priority);
        vector<uint16>* getPriorities()
        {
            return _ppriorities;
        }

        // Rendering the Atom in proper format (bitlengths, etc.) to an ostream
        virtual void renderToFileStream(ofstream &os);
        // Reading in the Atom components from an input stream
        virtual void readFromFileStream(ifstream &is);

    private:
        vector<uint16>* _ppriorities; // (15bit) with 1 bit padding when rendering

};


#endif // DEGRADATIONPRIORITYATOM_H_INCLUDED


