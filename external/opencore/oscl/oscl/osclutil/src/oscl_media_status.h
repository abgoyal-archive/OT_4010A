
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L _ M E D I A _ S T A T U S

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_MEDIA_STATUS_H
#define OSCL_MEDIA_STATUS_H


const int32 APPEND_MEDIA_AT_END = -1;

class BufFragStatusClass
{
    public:
        typedef enum
        {
            BFG_SUCCESS = 0,
            TOO_MANY_FRAGS = 1,
            NOT_ENOUGH_SPACE = 2,
            EMPTY_FRAGMENT = 3,
            NULL_INPUT = 4,
            FIXED_FRAG_LOC_FULL = 5,
            INTERNAL_ERROR,
            INVALID_ID
        } status_t;
};

class MediaStatusClass : public BufFragStatusClass {};

#endif

/*! @} */
