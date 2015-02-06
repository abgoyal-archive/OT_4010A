
#ifndef BUF_FRAG_GROUP
#define BUF_FRAG_GROUP 1

#include "oscl_media_data.h"

//
// Function seekBufFragGroup() moves the current position forward (if positive) or backward (if negative) by
// "delta_in_bytes". It returns false if error is encountered,
// else return true and change idx, offset and ptr to the new position.
// If boundary is reached, boundaryReached is set to true.
//
bool seekBufFragGroup(BufferFragment * fragGrp,
                      const int numFrags,
                      int & idx,
                      int & offset,
                      uint8 * & ptr,
                      bool & boundaryReached,
                      const int delta_in_bytes,
                      const int num_frags_to_skip);


#endif
