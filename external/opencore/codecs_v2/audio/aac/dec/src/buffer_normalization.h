

#ifndef BUFFER_NORMALIZATION_H
#define BUFFER_NORMALIZATION_H

#include "pv_audio_type_defs.h"


#define     ALL_ZEROS_BUFFER     -100





void buffer_normalization(
    Int     q_format,
    Int32   IO_buffer[],
    const Int     buffer_size,
    Int   * const pExp);

#endif  /* BUFFER_NORMALIZATION_H */
