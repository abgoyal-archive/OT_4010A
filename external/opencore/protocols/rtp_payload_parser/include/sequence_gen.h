
///////////////////////////////////////////////////////////////////////////////
//
// sequence_gen.h
//
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef SEQUENCE_GEN_H
#define SEQUENCE_GEN_H

#include "oscl_mem.h"
#include "oscl_vector.h"


#define SEQUENCEGEN_TIMESTAMP_REPOS_DELTA   10   // ms

class SequenceGenerator
{
    public:
        SequenceGenerator(int initialSeqNum = 0,
                          uint32 timestampReposDelta = SEQUENCEGEN_TIMESTAMP_REPOS_DELTA);
        virtual ~SequenceGenerator();

        bool registerNextObjectFrag(uint stream,
                                    uint objNum,
                                    uint objOffset,
                                    uint objLen,
                                    uint fragLen,
                                    bool* inCompleteFlag,
                                    bool ibRepositionFlag);

        uint32 generateSequenceNum(uint stream);
        uint32 generateTimestamp(uint stream, uint32 timestamp, bool reposition = false);

        uint32 getMinTimestamp();

        void   setSeqnum(const uint stream, const uint32 seqnum) ;// set next seqnum for stream
    private:

        struct StreamSequenceInfo
        {
            StreamSequenceInfo() : streamId(-1),
                    currObjectNum(0),
                    currObjectLen(0),
                    currObjectOffset(0),
                    objComplete(false),
                    currSeq(0),
                    timestampBase(0),
                    currTimestamp(0) { }

            int streamId;

            uint currObjectNum;
            uint currObjectLen;
            uint currObjectOffset;
            bool objComplete;

            uint32 currSeq;

            int32 timestampBase;
            uint32 currTimestamp;
        };

        StreamSequenceInfo* getStream(uint stream);

        int initialSeqNum;
        uint32 timestampReposDelta;

        Oscl_Vector<StreamSequenceInfo, OsclMemAllocator> vStreamInfo;
};

#endif // SEQUENCE_GEN_H
