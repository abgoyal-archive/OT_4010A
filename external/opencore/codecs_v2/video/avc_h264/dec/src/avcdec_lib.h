
#ifndef _AVCDEC_LIB_H_
#define _AVCDEC_LIB_H_

#include "avclib_common.h"
#include "avcdec_int.h"

/*----------- avcdec_api.c -------------*/
AVCDec_Status EBSPtoRBSP(uint8 *nal_unit, int *size);

/*------------- pred_intra.c ---------------*/
AVCStatus  IntraMBPrediction(AVCCommonObj *video);

void SaveNeighborForIntraPred(AVCCommonObj *video, int offset);

AVCStatus Intra_4x4(AVCCommonObj *video, int component, int SubBlock_indx, uint8 *comp);
void Intra_4x4_Vertical(AVCCommonObj *video, int block_offset);
void Intra_4x4_Horizontal(AVCCommonObj *video, int pitch, int block_offset);
void Intra_4x4_DC(AVCCommonObj *video, int pitch, int block_offset, AVCNeighborAvailability *availability);
void Intra_4x4_Down_Left(AVCCommonObj *video, int block_offset, AVCNeighborAvailability *availability);
void Intra_4x4_Diagonal_Down_Right(AVCCommonObj *video, int pitch, int block_offset);
void Intra_4x4_Diagonal_Vertical_Right(AVCCommonObj *video, int pitch, int block_offset);
void Intra_4x4_Diagonal_Horizontal_Down(AVCCommonObj *video, int pitch, int block_offset);
void Intra_4x4_Vertical_Left(AVCCommonObj *video,  int block_offset, AVCNeighborAvailability *availability);
void Intra_4x4_Horizontal_Up(AVCCommonObj *video, int pitch, int block_offset);
void  Intra_16x16_Vertical(AVCCommonObj *video);
void Intra_16x16_Horizontal(AVCCommonObj *video, int pitch);
void Intra_16x16_DC(AVCCommonObj *video, int pitch);
void Intra_16x16_Plane(AVCCommonObj *video, int pitch);
void Intra_Chroma_DC(AVCCommonObj *video, int pitch, uint8 *predCb, uint8 *predCr);
void  Intra_Chroma_Horizontal(AVCCommonObj *video, int pitch, uint8 *predCb, uint8 *predCr);
void  Intra_Chroma_Vertical(AVCCommonObj *video, uint8 *predCb, uint8 *predCr);
void  Intra_Chroma_Plane(AVCCommonObj *video, int pitch, uint8 *predCb, uint8 *predCr);

/*------------ pred_inter.c ---------------*/
void InterMBPrediction(AVCCommonObj *video);

void LumaMotionComp(uint8 *ref, int picwidth, int picheight,
                    int x_pos, int y_pos,
                    uint8 *pred, int pred_pitch,
                    int blkwidth, int blkheight);

void CreatePad(uint8 *ref, int picwidth, int picheight, int x_pos, int y_pos,
               uint8 *out, int blkwidth, int blkheight);

void FullPelMC(uint8 *in, int inwidth, uint8 *out, int outpitch,
               int blkwidth, int blkheight);

void HorzInterp1MC(uint8 *in, int inpitch, uint8 *out, int outpitch,
                   int blkwidth, int blkheight, int dx);

void HorzInterp2MC(int *in, int inpitch, uint8 *out, int outpitch,
                   int blkwidth, int blkheight, int dx);

void HorzInterp3MC(uint8 *in, int inpitch, int *out, int outpitch,
                   int blkwidth, int blkheight);

void VertInterp1MC(uint8 *in, int inpitch, uint8 *out, int outpitch,
                   int blkwidth, int blkheight, int dy);

void VertInterp2MC(uint8 *in, int inpitch, int *out, int outpitch,
                   int blkwidth, int blkheight);

void VertInterp3MC(int *in, int inpitch, uint8 *out, int outpitch,
                   int blkwidth, int blkheight, int dy);

void DiagonalInterpMC(uint8 *in1, uint8 *in2, int inpitch,
                      uint8 *out, int outpitch,
                      int blkwidth, int blkheight);


void ChromaMotionComp(uint8 *ref, int picwidth, int picheight,
                      int x_pos, int y_pos, uint8 *pred, int pred_pitch,
                      int blkwidth, int blkheight);

void ChromaFullPelMC(uint8 *in, int inpitch, uint8 *out, int outpitch,
                     int blkwidth, int blkheight) ;
void ChromaBorderMC(uint8 *ref, int picwidth, int dx, int dy,
                    uint8 *pred, int pred_pitch, int blkwidth, int blkheight);
void ChromaDiagonalMC(uint8 *ref, int picwidth, int dx, int dy,
                      uint8 *pred, int pred_pitch, int blkwidth, int blkheight);

void ChromaFullPelMCOutside(uint8 *ref, uint8 *pred, int pred_pitch,
                            int blkwidth, int blkheight, int x_inc,
                            int y_inc0, int y_inc1, int x_mid, int y_mid);
void ChromaBorderMCOutside(uint8 *ref, int picwidth, int dx, int dy,
                           uint8 *pred, int pred_pitch, int blkwidth, int blkheight,
                           int x_inc, int z_inc, int y_inc0, int y_inc1, int x_mid, int y_mid);
void ChromaDiagonalMCOutside(uint8 *ref, int picwidth,
                             int dx, int dy, uint8 *pred, int pred_pitch,
                             int blkwidth, int blkheight, int x_inc, int z_inc,
                             int y_inc0, int y_inc1, int x_mid, int y_mid);

void ChromaDiagonalMC_SIMD(uint8 *pRef, int srcPitch, int dx, int dy,
                           uint8 *pOut, int predPitch, int blkwidth, int blkheight);

void ChromaHorizontalMC_SIMD(uint8 *pRef, int srcPitch, int dx, int dy,
                             uint8 *pOut, int predPitch, int blkwidth, int blkheight);

void ChromaVerticalMC_SIMD(uint8 *pRef, int srcPitch, int dx, int dy,
                           uint8 *pOut, int predPitch, int blkwidth, int blkheight);

void ChromaFullMC_SIMD(uint8 *pRef, int srcPitch, int dx, int dy,
                       uint8 *pOut, int predPitch, int blkwidth, int blkheight);

void ChromaVerticalMC2_SIMD(uint8 *pRef, int srcPitch, int dx, int dy,
                            uint8 *pOut, int predPitch, int blkwidth, int blkheight);

void ChromaHorizontalMC2_SIMD(uint8 *pRef, int srcPitch, int dx, int dy,
                              uint8 *pOut, int predPitch, int blkwidth, int blkheight);

void ChromaDiagonalMC2_SIMD(uint8 *pRef, int srcPitch, int dx, int dy,
                            uint8 *pOut, int predPitch, int blkwidth, int blkheight);


/*----------- slice.c ---------------*/
AVCDec_Status DecodeSlice(AVCDecObject *video);
AVCDec_Status ConcealSlice(AVCDecObject *decvid, int mbnum_start, int mbnum_end);
AVCDec_Status DecodeMB(AVCDecObject *video);

AVCDec_Status mb_pred(AVCCommonObj *video, AVCMacroblock *currMB, AVCDecBitstream *stream);

AVCDec_Status sub_mb_pred(AVCCommonObj *video, AVCMacroblock *currMB, AVCDecBitstream *stream);

void InterpretMBModeI(AVCMacroblock *mblock, uint mb_type);

void InterpretMBModeP(AVCMacroblock *mblock, uint mb_type);

void InterpretMBModeB(AVCMacroblock *mblock, uint mb_type);

void InterpretMBModeSI(AVCMacroblock *mblock, uint mb_type);

void InterpretSubMBModeP(AVCMacroblock *mblock, uint *sub_mb_type);

void InterpretSubMBModeB(AVCMacroblock *mblock, uint *sub_mb_type);

AVCDec_Status DecodeIntra4x4Mode(AVCCommonObj *video, AVCMacroblock *currMB, AVCDecBitstream *stream);

/*----------- vlc.c -------------------*/
AVCDec_Status ue_v(AVCDecBitstream *bitstream, uint *codeNum);

AVCDec_Status  se_v(AVCDecBitstream *bitstream, int *value);

AVCDec_Status  se_v32bit(AVCDecBitstream *bitstream, int32 *value);

AVCDec_Status te_v(AVCDecBitstream *bitstream, uint *value, uint range);

AVCDec_Status GetEGBitstring(AVCDecBitstream *bitstream, int *leadingZeros, int *infobits);

AVCDec_Status GetEGBitstring32bit(AVCDecBitstream *bitstream, int *leadingZeros, uint32 *infobits);

AVCDec_Status DecodeCBP(AVCMacroblock *currMB, AVCDecBitstream *stream);

AVCDec_Status ce_TotalCoeffTrailingOnes(AVCDecBitstream *stream, int *TrailingOnes, int *TotalCoeff, int nC);

AVCDec_Status ce_TotalCoeffTrailingOnesChromaDC(AVCDecBitstream *stream, int *TrailingOnes, int *TotalCoeff);

AVCDec_Status code_from_bitstream_2d(AVCDecBitstream *stream, int *lentab, int *codtab, int tabwidth,
                                     int tabheight, int *code1, int *code2);

AVCDec_Status ce_LevelPrefix(AVCDecBitstream *stream, uint *code);

AVCDec_Status ce_TotalZeros(AVCDecBitstream *stream, int *code, int TotalCoeff);

AVCDec_Status ce_TotalZerosChromaDC(AVCDecBitstream *stream, int *code, int TotalCoeff);

AVCDec_Status ce_RunBefore(AVCDecBitstream *stream, int *code, int zeroLeft);

/*----------- header.c -------------------*/
AVCDec_Status vui_parameters(AVCDecObject *decvid, AVCDecBitstream *stream, AVCSeqParamSet *currSPS);
AVCDec_Status sei_payload(AVCDecObject *decvid, AVCDecBitstream *stream, uint payloadType, uint payloadSize);

AVCDec_Status buffering_period(AVCDecObject *decvid, AVCDecBitstream *stream);
AVCDec_Status pic_timing(AVCDecObject *decvid, AVCDecBitstream *stream);
AVCDec_Status recovery_point(AVCDecObject *decvid, AVCDecBitstream *stream);
AVCDec_Status dec_ref_pic_marking_repetition(AVCDecObject *decvid, AVCDecBitstream *stream);
AVCDec_Status motion_constrained_slice_group_set(AVCDecObject *decvid, AVCDecBitstream *stream);


AVCDec_Status hrd_parameters(AVCDecObject *decvid, AVCDecBitstream *stream, AVCHRDParams *HRDParam);

AVCDec_Status DecodeSPS(AVCDecObject *decvid, AVCDecBitstream *stream);

AVCDec_Status DecodePPS(AVCDecObject *decvid, AVCCommonObj *video, AVCDecBitstream *stream);
AVCDec_Status DecodeSEI(AVCDecObject *decvid, AVCDecBitstream *stream);

AVCDec_Status DecodeSliceHeader(AVCDecObject *decvid, AVCCommonObj *video, AVCDecBitstream *stream);

AVCDec_Status fill_frame_num_gap(AVCHandle *avcHandle, AVCCommonObj *video);

AVCDec_Status ref_pic_list_reordering(AVCCommonObj *video, AVCDecBitstream *stream, AVCSliceHeader *sliceHdr, int slice_type);

AVCDec_Status dec_ref_pic_marking(AVCCommonObj *video, AVCDecBitstream *stream, AVCSliceHeader *sliceHdr);

AVCDec_Status DecodePOC(AVCCommonObj *video);



/*------------ residual.c ------------------*/
AVCDec_Status DecodeIntraPCM(AVCCommonObj *video, AVCDecBitstream *stream);

AVCDec_Status residual(AVCDecObject *video, AVCMacroblock *currMB);

AVCDec_Status residual_block_cavlc(AVCDecObject *video, int nC, int maxNumCoeff,
                                   int *level, int *run, int *numcoeff);

#endif /* _AVCDEC_LIB_H_ */
