
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                      MPEG-4 ChunkLargeOffsetAtom Class                        */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef CHUNKLARGEOFFSETATOM_H_INCLUDED
#define CHUNKLARGEOFFSETATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef FULLATOM_H_INCLUDED
#include "fullatom.h"
#endif

class ChunkLargeOffsetAtom : public FullAtom
{

    public:
        ChunkLargeOffsetAtom(uint8 version, uint32 flags); // Constructor
        ChunkLargeOffsetAtom(ChunkLargeOffsetAtom atom); // Copy Constructor
        virtual ~ChunkLargeOffsetAtom();

        // Member gets and sets
        uint32 getEntryCount()
        {
            return _entryCount;
        }
        void setEntryCount(uint32 count)
        {
            _entryCount = count;
        }

        // Adding to and getting first chunk offset values
        void addChunkOffset(uint64 offset);
        vector<uint64>* getChunkOffsets()
        {
            return _pchunkOffsets;
        }

        // Rendering the Atom in proper format (bitlengths, etc.) to an ostream
        virtual void renderToFileStream(ofstream &os);
        // Reading in the Atom components from an input stream
        virtual void readFromFileStream(ifstream &is);


    private:
        uint32 _entryCount;
        vector<uint64>* _pchunkOffsets;

};


#endif // CHUNKLARGEOFFSETATOM_H_INCLUDED
