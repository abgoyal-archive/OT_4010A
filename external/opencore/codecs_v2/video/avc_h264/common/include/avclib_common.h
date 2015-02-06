
#ifndef AVCCOMMON_LIB_H_INCLUDED
#define AVCCOMMON_LIB_H_INCLUDED

#ifndef AVCINT_COMMON_H_INCLUDED
#include "avcint_common.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

/*----------- deblock.c --------------*/
OSCL_IMPORT_REF AVCStatus DeblockPicture(AVCCommonObj *video);

void MBInLoopDeblock(AVCCommonObj *video);


/*---------- dpb.c --------------------*/
OSCL_IMPORT_REF AVCStatus AVCConfigureSequence(AVCHandle *avcHandle, AVCCommonObj *video, bool padding);

AVCStatus InitDPB(AVCHandle *avcHandle, AVCCommonObj *video, int FrameHeightInMbs, int PicWidthInMbs, bool padding);

OSCL_IMPORT_REF AVCStatus CleanUpDPB(AVCHandle *avcHandle, AVCCommonObj *video);

OSCL_IMPORT_REF AVCStatus DPBInitBuffer(AVCHandle *avcHandle, AVCCommonObj *video);

OSCL_IMPORT_REF void DPBInitPic(AVCCommonObj *video, int CurrPicNum);

OSCL_IMPORT_REF void DPBReleaseCurrentFrame(AVCHandle *avcHandle, AVCCommonObj *video);

OSCL_IMPORT_REF AVCStatus StorePictureInDPB(AVCHandle *avcHandle, AVCCommonObj *video);

AVCStatus sliding_window_process(AVCHandle *avcHandle, AVCCommonObj *video, AVCDecPicBuffer *dpb);


AVCStatus adaptive_memory_marking(AVCHandle *avcHandle, AVCCommonObj *video, AVCDecPicBuffer *dpb, AVCSliceHeader *sliceHdr);

void MemMgrCtrlOp1(AVCHandle *avcHandle, AVCCommonObj *video, AVCDecPicBuffer *dpb, int difference_of_pic_nums_minus1);

void MemMgrCtrlOp2(AVCHandle *avcHandle, AVCDecPicBuffer *dpb, int long_term_pic_num);

void MemMgrCtrlOp3(AVCHandle *avcHandle, AVCCommonObj *video, AVCDecPicBuffer *dpb, uint difference_of_pic_nums_minus1,
                   uint long_term_frame_idx);

void MemMgrCtrlOp4(AVCHandle *avcHandle, AVCCommonObj *video, AVCDecPicBuffer *dpb, uint max_long_term_frame_idx_plus1);

void MemMgrCtrlOp5(AVCHandle *avcHandle, AVCCommonObj *video, AVCDecPicBuffer *dpb);

void MemMgrCtrlOp6(AVCHandle *avcHandle, AVCCommonObj *video, AVCDecPicBuffer *dpb, uint long_term_frame_idx);

void unmark_long_term_frame_for_reference_by_frame_idx(AVCHandle *avcHandle, AVCDecPicBuffer *dpb, uint long_term_frame_idx);

void unmark_long_term_field_for_reference_by_frame_idx(AVCCommonObj *video, AVCDecPicBuffer *dpb, uint long_term_frame_indx, int picNumX);

void unmark_for_reference(AVCHandle *avcHandle, AVCDecPicBuffer *dpb, uint idx);

void update_ref_list(AVCDecPicBuffer *dpb);


/*---------- fmo.c --------------*/
OSCL_IMPORT_REF AVCStatus FMOInit(AVCCommonObj *video);

void FmoGenerateType0MapUnitMap(int *mapUnitToSliceGroupMap, uint *run_length_minus1, uint num_slice_groups_minus1, uint PicSizeInMapUnits);

void FmoGenerateType1MapUnitMap(int *mapUnitToSliceGroupMap, int PicWidthInMbs, uint num_slice_groups_minus1, uint PicSizeInMapUnits);

void FmoGenerateType2MapUnitMap(AVCPicParamSet *pps, int *mapUnitToSliceGroupMap, int PicWidthInMbs,
                                uint num_slice_groups_minus1, uint PicSizeInMapUnits);

void FmoGenerateType3MapUnitMap(AVCCommonObj *video, AVCPicParamSet* pps, int *mapUnitToSliceGroupMap,
                                int PicWidthInMbs);

void FmoGenerateType4MapUnitMap(int *mapUnitToSliceGroupMap, int MapUnitsInSliceGroup0,
                                int slice_group_change_direction_flag, uint PicSizeInMapUnits);

void FmoGenerateType5MapUnitMap(int *mapUnitsToSliceGroupMap, AVCCommonObj *video,
                                int slice_group_change_direction_flag, uint PicSizeInMapUnits);

void FmoGenerateType6MapUnitMap(int *mapUnitsToSliceGroupMap, int *slice_group_id, uint PicSizeInMapUnits);

/*------------- itrans.c --------------*/
void Intra16DCTrans(int16 *block, int Qq, int Rq);

void itrans(int16 *block, uint8 *pred, uint8 *cur, int width);

void ictrans(int16 *block, uint8 *pred, uint8 *cur, int width);

void ChromaDCTrans(int16 *block, int Qq, int Rq);

void copy_block(uint8 *pred, uint8 *cur, int width, int pred_pitch);

/*--------- mb_access.c ----------------*/
OSCL_IMPORT_REF void InitNeighborAvailability(AVCCommonObj *video, int mbNum);

bool mb_is_available(AVCMacroblock *mblock, uint PicSizeInMbs, int mbAddr, int currMbAddr);

OSCL_IMPORT_REF int predict_nnz(AVCCommonObj *video, int i, int j);

OSCL_IMPORT_REF int predict_nnz_chroma(AVCCommonObj *video, int i, int j);

OSCL_IMPORT_REF void GetMotionVectorPredictor(AVCCommonObj *video, int encFlag);

/*---------- reflist.c -----------------*/
OSCL_IMPORT_REF void RefListInit(AVCCommonObj *video);

void    GenPicListFromFrameList(AVCCommonObj *video, int IsL1, int long_term);

OSCL_IMPORT_REF AVCStatus ReOrderList(AVCCommonObj *video);

AVCStatus ReorderRefPicList(AVCCommonObj *video, int isL1);

AVCStatus ReorderShortTerm(AVCCommonObj *video, int picNumLX, int *refIdxLX, int isL1);

AVCStatus ReorderLongTerm(AVCCommonObj *video, int LongTermPicNum, int *refIdxLX, int isL1);

AVCPictureData*  GetShortTermPic(AVCCommonObj *video, int picNum);

AVCPictureData*  GetLongTermPic(AVCCommonObj *video, int LongtermPicNum);

int is_short_ref(AVCPictureData *s);

int is_long_ref(AVCPictureData *s);

void SortPicByPicNum(AVCPictureData *data[], int num);

void SortPicByPicNumLongTerm(AVCPictureData *data[], int num);

void SortFrameByFrameNumWrap(AVCFrameStore *data[], int num);

void SortFrameByLTFrameIdx(AVCFrameStore *data[], int num);

void SortPicByPOC(AVCPictureData *data[], int num, int descending);

void SortPicByLTPicNum(AVCPictureData *data[], int num);

void SortFrameByPOC(AVCFrameStore *data[], int num, int descending);


#endif /* _AVCCOMMON_LIB_H_ */
