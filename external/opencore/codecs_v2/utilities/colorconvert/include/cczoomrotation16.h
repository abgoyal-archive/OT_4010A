
#ifndef CCZOOMROTATION16_H_INCLUDED
#define CCZOOMROTATION16_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef CCZOOMROTATIONBASE_H_INCLUDED
#include "cczoomrotationbase.h"
#endif


class ColorConvert16: public ColorConvertBase
{
    public:

        OSCL_IMPORT_REF static ColorConvertBase* NewL(void);
        OSCL_IMPORT_REF virtual ~ColorConvert16();
        int32 Init(int32 Src_width, int32 Src_height, int32 Src_pitch, int32 Dst_width, int32 Dst_height, int32 Dst_pitch, int32 nRotation = 0);
        int32 SetYuvFullRange(bool range);
        int32 SetMode(int32 nMode); //nMode : 0 Off, 1 On
        int32 Convert(uint8 *srcBuf, uint8 *destBuf);
        int32 Convert(uint8 **srcBuf, uint8 *destBuf);
        int32 GetOutputBufferSize(void);


    private:
        ColorConvert16();

        uint32 mCoefTbl32[516];
        uint8 *mCoefTbl;
        int32 get_frame16(uint8 **src, uint8 *dst, DisplayProperties *disp, uint8 *COFF_TBL);

        int32 cc16Rotate(uint8 **src, uint8 *dst, DisplayProperties *disp, uint8 *COFF_TBL);
        int32 cc16ZoomIn(uint8 **src, uint8 *dst, DisplayProperties *disp, uint8 *COFF_TBL);
        int32 cc16ZoomRotate(uint8 **src, uint8 *dst, DisplayProperties *disp, uint8 *COFF_TBL);
        
    private:
        int32(ColorConvert16::*mPtrYUV2RGB)(uint8 **src, uint8 *dst, DisplayProperties *disp, uint8 *COFF_TBL);

        // add by Ryan
        void MTKYUVToYUV420Conversion(uint8 * input_buffer, uint8 * output_buffer);      

        // add by Jackal
        int32 SWYUVToRGBConversion(uint8 *srcBuf, uint8 *destBuf);
        int32 HWYUVToRGBConversion(uint8 *srcBuf, uint8 *destBuf);
};

#endif // CCZOOMROTATION16_H_INCLUDED
