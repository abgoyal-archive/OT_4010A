
/*********************************************************************************/
/*     -------------------------------------------------------------------       */
/*                      MPEG-4 ShadowSyncSampleAtom Class                        */
/*     -------------------------------------------------------------------       */
/*********************************************************************************/


#ifndef SHADOWSYNCSAMPLEATOM_H_INCLUDED
#define SHADOWSYNCSAMPLEATOM_H_INCLUDED

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef FULLATOM_H_INCLUDED
#include "fullatom.h"
#endif

class ShadowSyncSampleAtom : public FullAtom
{

    public:
        ShadowSyncSampleAtom(uint8 version, uint32 flags); // Constructor
        ShadowSyncSampleAtom(ShadowSyncSampleAtom atom); // Copy Constructor
        virtual ~ShadowSyncSampleAtom();

        // Member gets and sets
        uint32 getEntryCount()
        {
            return _entryCount;
        }

        // Adding to and getting the shadow and sync sample number values
        void addShadowSampleNumber(uint32 sample);
        vector<uint32>* getShadowSampleNumbers()
        {
            return _pshadowSampleNumbers;
        }
        uint32 getShadowSampleNumberAt(int32 index);

        void addSyncSampleNumber(uint32 sample);
        vector<uint32>* getSyncSampleNumbers()
        {
            return _psyncSampleNumbers;
        }
        uint32 getSyncSampleNUmberAt(int32 index);

        // Rendering the Atom in proper format (bitlengths, etc.) to an ostream
        virtual void renderToFileStream(ofstream &os);
        // Reading in the Atom components from an input stream
        virtual void readFromFileStream(ifstream &is);

    private:
        uint32 _entryCount;
        vector<uint32>* _pshadowSampleNumbers;
        vector<uint32>* _psyncSampleNumbers;

};

#endif // SHADOWSYNCSAMPLEATOM_H_INCLUDED


