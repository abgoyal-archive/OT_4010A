
#ifndef CCZOOMROTATION12_H_INCLUDED
#define CCZOOMROTATION12_H_INCLUDED


#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef CCZOOMROTATIONBASE_H_INCLUDED
#include "cczoomrotationbase.h"
#endif

class ColorConvert12: public ColorConvertBase
{
    public:
        OSCL_IMPORT_REF static ColorConvertBase* NewL(void);
        OSCL_IMPORT_REF virtual ~ColorConvert12();

        int32 Init(int32 Src_width, int32 Src_height, int32 Src_pitch, int32 Dst_width, int32 Dst_height, int32 Dst_pitch, int32 nRotation = 0);
        int32 SetYuvFullRange(bool range);
        int32 SetMode(int32 nMode); //nMode : 0 Off, 1 On
        int32 Convert(uint8 **srcBuf, uint8 *destBuf);
        int32 Convert(uint8 *srcBuf, uint8 *destBuf);
        int32 GetOutputBufferSize(void);

    private:
        ColorConvert12();

        int32 get_frame12(uint8 **src, uint8 *dst, DisplayProperties *disp, uint8 *COFF_TBL, uint8 *pExtraParam = 0);

        int32 cc12Rotate(uint8 **src, uint8 *dst, DisplayProperties *disp, uint8 *COFF_TBL, uint8 *pExtraParam = 0);
        int32 cc12ZoomIn(uint8 **src, uint8 *dst, DisplayProperties *disp, uint8 *COFF_TBL, uint8 *pExtraParam = 0);
        int32 cc12ZoomRotate(uint8 **src, uint8 *dst, DisplayProperties *disp, uint8 *COFF_TBL, uint8 *pExtraParam = 0);

    private:
        uint8 *mErr_horz[3], *mClip;
        int32(ColorConvert12::*mPtrYUV2RGB)(uint8 **src, uint8 *dst, DisplayProperties *disp, uint8 *COFF_TBL, uint8 *pExtraParam);
};

#endif // CCZOOMROTATION12_H_INCLUDED

