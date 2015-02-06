



#ifndef _VLCDECODE_H_
#define _VLCDECODE_H_

#include "mp4lib_int.h"

#define VLC_ERROR_DETECTED(x) ((x) < 0)
#define VLC_IO_ERROR    -1
#define VLC_CODE_ERROR  -2
#define VLC_MB_STUFFING -4
#define VLC_NO_LAST_BIT -5

#define VLC_ESCAPE_CODE  7167

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    PV_STATUS DecodeUserData(BitstreamDecVideo *stream);
    PV_STATUS PV_GetMBvectors(VideoDecData *, uint mode);
    PV_STATUS PV_DecodeMBVec(BitstreamDecVideo *stream, MOT *mv_x, MOT *mv_y, int f_code_f);
    PV_STATUS PV_DeScaleMVD(int f_code, int residual, int vlc_code_mag,  MOT *vector);

    PV_STATUS PV_VlcDecMV(BitstreamDecVideo *stream, int *mv);
    int PV_VlcDecMCBPC_com_intra(BitstreamDecVideo *stream);
    int PV_VlcDecMCBPC_com_inter(BitstreamDecVideo *stream);
#ifdef PV_ANNEX_IJKT_SUPPORT
    int PV_VlcDecMCBPC_com_inter_H263(BitstreamDecVideo *stream);
    PV_STATUS VlcDecTCOEFShortHeader_AnnexI(BitstreamDecVideo *stream, Tcoef *pTcoef);
    PV_STATUS VlcDecTCOEFShortHeader_AnnexT(BitstreamDecVideo *stream, Tcoef *pTcoef); /* ANNEX_T */
    PV_STATUS VlcDecTCOEFShortHeader_AnnexIT(BitstreamDecVideo *stream, Tcoef *pTcoef);
#endif
    int PV_VlcDecCBPY(BitstreamDecVideo *stream, int intra);

    PV_STATUS VlcDecTCOEFIntra(BitstreamDecVideo *stream, Tcoef *pTcoef);
    PV_STATUS VlcDecTCOEFInter(BitstreamDecVideo *stream, Tcoef *pTcoef);
    PV_STATUS VlcDecTCOEFShortHeader(BitstreamDecVideo *stream, Tcoef *pTcoef);
    PV_STATUS RvlcDecTCOEFIntra(BitstreamDecVideo *stream, Tcoef *pTcoef);
    PV_STATUS RvlcDecTCOEFInter(BitstreamDecVideo *stream, Tcoef *pTcoef);
    PV_STATUS PV_VlcDecIntraDCPredSize(BitstreamDecVideo *stream, int compnum, uint *DC_size);

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif

