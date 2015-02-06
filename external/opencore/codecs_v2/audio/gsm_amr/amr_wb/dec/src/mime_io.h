

#ifndef MIME_IO_H
#define MIME_IO_H








extern const uint8 toc_byte[16];

/* number of speech bits for all modes */
extern const int16 unpacked_size[16];

/* size of packed frame for each mode, excluding TOC byte */
extern const int16 packed_size[16];

/* number of unused speech bits in packed format for each mode */
extern const int16 unused_size[16];

/* sorting tables for all modes */

extern const int16 sort_660[132];

extern const int16 sort_885[177];

extern const int16 sort_1265[253];

extern const int16 sort_1425[285];

extern const int16 sort_1585[317];

extern const int16 sort_1825[365];

extern const int16 sort_1985[397];

extern const int16 sort_2305[461];

extern const int16 sort_2385[477];

extern const int16 sort_SID[35];





#ifdef __cplusplus
extern "C"
{
#endif


#ifdef __cplusplus
}
#endif




#endif  /* MIME_IO_H */
