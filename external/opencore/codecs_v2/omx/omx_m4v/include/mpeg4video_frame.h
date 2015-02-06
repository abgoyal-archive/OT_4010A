

#include "mp4dec_api.h"

int InitializeVideoDecode(int32 *width, int32 *height,
                          unsigned char **buffer, int32 *size, int32 *max_size, int postproc);

void DisplayVideoFrame(unsigned char *frame, unsigned char *ppvBits);
int EndVideoDecode();

