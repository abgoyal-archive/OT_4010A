

#ifndef COLORCONV_CONFIG_H_INCLUDED
#define COLORCONV_CONFIG_H_INCLUDED

/** For scaling support, define CCSCALING to 1, else set it to 0 */
#define CCSCALING 1

/** For rotation support, define CCROTATE to 1, else set it to 0  */
#define CCROTATE 1

/** To specify RGB format. define RGB_FORMAT to 1. For, BGR format, set it to 0 */
#define RGB_FORMAT  0

/* Set one of the following to 1  depending on your requirement */
#ifdef SHOLES_PROPERTY_OVERRIDES
#define ENDIAN_1 1
#define ENDIAN_2 0
#else
#define ENDIAN_1 0
#define ENDIAN_2 1
#endif
#define ENDIAN_3 0

#endif // COLORCONV_CONFIG_H_INCLUDED


