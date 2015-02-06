
#ifndef PV_AVIFILE_INDX_H_INCLUDED
#define PV_AVIFILE_INDX_H_INCLUDED

#ifndef PV_AVIFILE_TYPEDEFS_H_INCLUDED
#include "pv_avifile_typedefs.h"
#endif

#ifndef PV_AVIFILE_PARSER_UTILS_H_INCLUDED
#include "pv_avifile_parser_utils.h"
#endif

typedef Oscl_Vector<IdxTblType, OsclMemAllocator> IndxTblVector;

//class to parse values in index chunk. index chunk contains offset and size info of media sample.
class PVAviFileIdxChunk: public PVAviFileParserStatus
{

    public:

        //constructor
        PVAviFileIdxChunk(PVFile* aFp, uint32 aIndxSize, uint32 aNumStreams);

        //destructor
        ~PVAviFileIdxChunk() { };

        uint32 GetOffset(uint32 aStreamNo, uint32 aSampleNo)
        {
            return (((iIndexTable[aStreamNo])[aSampleNo]).offset);
        }

        uint32 GetSampleSize(uint32 aStreamNo, uint32 aSampleNo)
        {
            return (((iIndexTable[aStreamNo])[aSampleNo]).size);
        }

        uint32 GetNumberOfSamplesInStream(uint32 aStreamNo)
        {
            return ((iIndexTable[aStreamNo]).size());
        }

        bool IsOffsetFromMoviList()
        {
            return iOffsetFrmMoviLst;
        }

        Oscl_Vector<IndxTblVector, OsclMemAllocator>
        GetIndexTable()
        {
            return iIndexTable;
        }

    private:

        // variable iIndexTable stores the offset and size info of ever sample in a stream
        // size of IndxTblTypeVector = number of samples ("movi" Subchunks) in a stream.
        // size of iIndexTable = number of streams.

        Oscl_Vector < IndxTblVector,
        OsclMemAllocator >    iIndexTable;
        uint32                           iIndexSize;
        uint32                           iNumStreams;
        bool                             iOffsetFrmMoviLst;

};

#endif //#ifndef PV_AVIFILE_INDX_H_INCLUDED

