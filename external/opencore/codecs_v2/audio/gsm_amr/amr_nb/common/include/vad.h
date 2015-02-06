
#ifndef vad_h
#define vad_h "$Id $"


#include "vad1.h"   /* for VAD option 1 */
#include "vad2.h"   /* for VAD option 2 */



#ifndef VAD2
#define vadState vadState1
#else
#define vadState vadState2
#endif


#endif
