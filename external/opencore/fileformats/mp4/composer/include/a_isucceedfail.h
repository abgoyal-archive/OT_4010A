

#ifndef A_ISUCCEEDFAIL_H_INCLUDED
#define A_ISUCCEEDFAIL_H_INCLUDED

#include "oscl_types.h"

typedef enum {  DEFAULT_ERROR   = -1,
                EVERYTHING_FINE,
                READ_FAILED,
                READ_USER_DATA_ATOM_FAILED,
                READ_PV_USER_DATA_ATOM_FAILED,
                READ_MEDIA_DATA_ATOM_FAILED,
                READ_MOVIE_ATOM_FAILED,
                READ_TRACK_ATOM_FAILED,
                READ_OBJECT_DESCRIPTOR_ATOM_FAILED,
                DUPLICATE_MOVIE_ATOMS,
                DUPLICATE_OBJECT_DESCRIPTORS,
                DUPLICATE_MOVIE_HEADERS,
                READ_UNKNOWN_ATOM,
                MEMORY_ALLOCATION_FAILED
             } MP4_ERROR_CODE;

class PVA_FF_ISucceedFail
{

    public:
        bool Mp4Success()
        {
            return _success;
        }
        bool MP4Success()
        {
            return _success;
        }
        int GetMP4Error()
        {
            return _mp4ErrorCode;
        }

    protected:
        bool _success;
        int _mp4ErrorCode;
};

#endif
