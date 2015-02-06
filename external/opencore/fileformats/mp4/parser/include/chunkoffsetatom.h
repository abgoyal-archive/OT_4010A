


#ifndef CHUNKOFFSETATOM_H_INCLUDED
#define CHUNKOFFSETATOM_H_INCLUDED

#define PV_ERROR -1

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif
#ifndef FULLATOM_H_INCLUDED
#include "fullatom.h"
#endif

class ChunkOffsetAtom : public FullAtom
{

    public:
        ChunkOffsetAtom(MP4_FF_FILE *fp, uint32 size, uint32 type,
                        OSCL_wString& filename,
                        uint32 parsingMode);
        virtual ~ChunkOffsetAtom();

        // Member gets and sets
        uint32 getEntryCount() const
        {
            return _entryCount;
        }

        // Getting first chunk offset values
        int32 getChunkOffsetAt(int32 index) ;
        int32 getChunkClosestToOffset(uint32 offSet, int32& index);


    private:
        bool ParseEntryUnit(uint32 sample_cnt);
        uint32 _entryCount;
        uint32 *_pchunkOffsets;

        int32 _mediaType;
        uint32 _currentDataOffset;
        MP4_FF_FILE *_fileptr;
        uint32  _parsed_entry_cnt;

        MP4_FF_FILE *_curr_fptr;
        uint32 *_stbl_fptr_vec;
        uint32 _stbl_buff_size;
        uint32 _curr_entry_point;
        uint32 _curr_buff_number;
        uint32 _next_buff_number;
        uint32 _parsingMode;
        uint32 _head_offset;

};

#endif // CHUNKOFFSETATOM_H_INCLUDED

