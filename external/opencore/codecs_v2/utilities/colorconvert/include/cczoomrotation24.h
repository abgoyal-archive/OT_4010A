
#ifndef CCZOOMROTATION24_H_INCLUDED
#define CCZOOMROTATION24_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef CCZOOMROTATIONBASE_H_INCLUDED
#include "cczoomrotationbase.h"
#endif

class ColorConvert24: public ColorConvertBase
{
    public:
        OSCL_IMPORT_REF static ColorConvertBase* NewL(void);
        OSCL_IMPORT_REF virtual ~ColorConvert24();

        int32 Init(int32 Src_width, int32 Src_height, int32 Src_pitch, int32 Dst_width, int32 Dst_height, int32 Dst_pitch, int32 nRotation = 0);
        int32 SetMode(int nMode);   //nMode : 0 Off, 1 On
        int32 SetYuvFullRange(bool range);
        int32 Convert(uint8 *srcBuf, uint8 *destBuf);
        int32 Convert(uint8 **srcBuf, uint8 *destBuf);
        int32 GetOutputBufferSize(void);

    private:
        ColorConvert24();

        uint32 mCoefTbl32[260];
        uint8  *mClip;

        int32 get_frame24(uint8 **src, uint8 *dst, DisplayProperties *disp, uint8 *COFF_TBL);

        int32 cc24Rotate(uint8 **src, uint8 *dst, DisplayProperties *disp, uint8 *COFF_TBL);
        int32 cc24ZoomIn(uint8 **src, uint8 *dst, DisplayProperties *disp, uint8 *COFF_TBL);
        int32 cc24ZoomRotate(uint8 **src, uint8 *dst, DisplayProperties *disp, uint8 *COFF_TBL);

    private:
        int32(ColorConvert24::*mPtrYUV2RGB)(uint8 **src, uint8 *dst, DisplayProperties *disp, uint8 *COFF_TBL);

};

#endif // CCZOOMROTATION24_H_INCLUDED

