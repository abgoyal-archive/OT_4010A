

#ifndef d_homing_h
#define d_homing_h "$Id $"


#include "typedef.h"
#include "mode.h"


Word16 decoder_homing_frame_test(Word16 input_frame[], enum Mode mode);
Word16 decoder_homing_frame_test_first(Word16 input_frame[], enum Mode mode);

#endif
