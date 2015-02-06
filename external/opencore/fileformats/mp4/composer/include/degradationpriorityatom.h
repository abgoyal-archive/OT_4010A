


#ifndef __DegradationPriorityAtom_H__
#define __DegradationPriorityAtom_H__

#include "fullatom.h"


class PVA_FF_DegradationPriorityAtom : public PVA_FF_FullAtom
{

    public:
        PVA_FF_DegradationPriorityAtom(uint8 version, uint32 flags); // Constructor

        virtual ~PVA_FF_DegradationPriorityAtom();

        // Adding to and getting the sample priority values
        void addPriority(uint16 priority);
        Oscl_Vector<uint16, OsclMemAllocator>* getPriorities()
        {
            return _ppriorities;
        }

        // Rendering the PVA_FF_Atom in proper format (bitlengths, etc.) to an ostream
        virtual bool renderToFileStream(MP4_AUTHOR_FF_FILE_IO_WRAP *fp);

    private:

        Oscl_Vector<uint16, OsclMemAllocator>* _ppriorities;
};


#endif

