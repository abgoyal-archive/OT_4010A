

#ifndef OMX_Video_h
#define OMX_Video_h


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



#include <OMX_IVCommon.h>


typedef enum OMX_VIDEO_CODINGTYPE {
    OMX_VIDEO_CodingUnused,     /**< Value when coding is N/A */
    OMX_VIDEO_CodingAutoDetect, /**< Autodetection of coding type */
    OMX_VIDEO_CodingMPEG2,      /**< AKA: H.262 */
    OMX_VIDEO_CodingH263,       /**< H.263 */
    OMX_VIDEO_CodingMPEG4,      /**< MPEG-4 */
    OMX_VIDEO_CodingWMV,        /**< all versions of Windows Media Video */
    OMX_VIDEO_CodingRV,         /**< all versions of Real Video */
    OMX_VIDEO_CodingAVC,        /**< H.264/AVC */
    OMX_VIDEO_CodingMJPEG,      /**< Motion JPEG */
    OMX_VIDEO_CodingVPX,        /**< VPX added by Legis Lu 20110714*/ 
    OMX_VIDEO_CodingKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_VIDEO_CodingVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_VIDEO_CodingS263,       /**< Soreson 263 */
    OMX_VIDEO_CodingDIVX,       /**< DIVX MPEG-4 */
    OMX_VIDEO_CodingXVID,       /**< XVID MPEG-4 */
    OMX_VIDEO_CodingMax = 0x7FFFFFFF
} OMX_VIDEO_CODINGTYPE;


typedef struct OMX_VIDEO_PORTDEFINITIONTYPE {
    OMX_STRING cMIMEType;
    OMX_NATIVE_DEVICETYPE pNativeRender;
    OMX_U32 nFrameWidth;
    OMX_U32 nFrameHeight;
    OMX_S32 nStride;
    OMX_U32 nSliceHeight;
    OMX_U32 nBitrate;
    OMX_U32 xFramerate;
    OMX_BOOL bFlagErrorConcealment;
    OMX_VIDEO_CODINGTYPE eCompressionFormat;
    OMX_COLOR_FORMATTYPE eColorFormat;
    OMX_NATIVE_WINDOWTYPE pNativeWindow;
} OMX_VIDEO_PORTDEFINITIONTYPE;

typedef struct OMX_VIDEO_PARAM_PORTFORMATTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nIndex;
    OMX_VIDEO_CODINGTYPE eCompressionFormat; 
    OMX_COLOR_FORMATTYPE eColorFormat;
    OMX_U32 xFramerate;
} OMX_VIDEO_PARAM_PORTFORMATTYPE;


typedef struct OMX_VIDEO_PARAM_QUANTIZATIONTYPE {
    OMX_U32 nSize;            
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nQpI;
    OMX_U32 nQpP;
    OMX_U32 nQpB;
} OMX_VIDEO_PARAM_QUANTIZATIONTYPE;


typedef struct OMX_VIDEO_PARAM_VIDEOFASTUPDATETYPE {
    OMX_U32 nSize;            
    OMX_VERSIONTYPE nVersion; 
    OMX_U32 nPortIndex;       
    OMX_BOOL bEnableVFU;      
    OMX_U32 nFirstGOB;                            
    OMX_U32 nFirstMB;                            
    OMX_U32 nNumMBs;                                  
} OMX_VIDEO_PARAM_VIDEOFASTUPDATETYPE;


typedef enum OMX_VIDEO_CONTROLRATETYPE {
    OMX_Video_ControlRateDisable,
    OMX_Video_ControlRateVariable,
    OMX_Video_ControlRateConstant,
    OMX_Video_ControlRateVariableSkipFrames,
    OMX_Video_ControlRateConstantSkipFrames,
    OMX_Video_ControlRateKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_Video_ControlRateVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_Video_ControlRateMax = 0x7FFFFFFF
} OMX_VIDEO_CONTROLRATETYPE;


typedef struct OMX_VIDEO_PARAM_BITRATETYPE {
    OMX_U32 nSize;                          
    OMX_VERSIONTYPE nVersion;               
    OMX_U32 nPortIndex;                     
    OMX_VIDEO_CONTROLRATETYPE eControlRate; 
    OMX_U32 nTargetBitrate;                 
} OMX_VIDEO_PARAM_BITRATETYPE;


typedef enum OMX_VIDEO_MOTIONVECTORTYPE {
    OMX_Video_MotionVectorPixel,
    OMX_Video_MotionVectorHalfPel,
    OMX_Video_MotionVectorQuarterPel,
    OMX_Video_MotionVectorEighthPel,
    OMX_Video_MotionVectorKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_Video_MotionVectorVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_Video_MotionVectorMax = 0x7FFFFFFF
} OMX_VIDEO_MOTIONVECTORTYPE;


typedef struct OMX_VIDEO_PARAM_MOTIONVECTORTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_VIDEO_MOTIONVECTORTYPE eAccuracy;
    OMX_BOOL bUnrestrictedMVs;
    OMX_BOOL bFourMV;
    OMX_S32 sXSearchRange;
    OMX_S32 sYSearchRange;
} OMX_VIDEO_PARAM_MOTIONVECTORTYPE;


typedef enum OMX_VIDEO_INTRAREFRESHTYPE {
    OMX_VIDEO_IntraRefreshCyclic,
    OMX_VIDEO_IntraRefreshAdaptive,
    OMX_VIDEO_IntraRefreshBoth,
    OMX_VIDEO_IntraRefreshKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_VIDEO_IntraRefreshVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_VIDEO_IntraRefreshMax = 0x7FFFFFFF
} OMX_VIDEO_INTRAREFRESHTYPE;


typedef struct OMX_VIDEO_PARAM_INTRAREFRESHTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_VIDEO_INTRAREFRESHTYPE eRefreshMode;
    OMX_U32 nAirMBs;
    OMX_U32 nAirRef;
    OMX_U32 nCirMBs;
} OMX_VIDEO_PARAM_INTRAREFRESHTYPE;


typedef struct OMX_VIDEO_PARAM_ERRORCORRECTIONTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bEnableHEC;
    OMX_BOOL bEnableResync;
    OMX_U32  nResynchMarkerSpacing;
    OMX_BOOL bEnableDataPartitioning;
    OMX_BOOL bEnableRVLC;
} OMX_VIDEO_PARAM_ERRORCORRECTIONTYPE;


typedef struct OMX_VIDEO_PARAM_VBSMCTYPE {
    OMX_U32 nSize; 
    OMX_VERSIONTYPE nVersion; 
    OMX_U32 nPortIndex;       
    OMX_BOOL b16x16; 
    OMX_BOOL b16x8; 
    OMX_BOOL b8x16;
    OMX_BOOL b8x8;
    OMX_BOOL b8x4;
    OMX_BOOL b4x8;
    OMX_BOOL b4x4;
} OMX_VIDEO_PARAM_VBSMCTYPE;


typedef enum OMX_VIDEO_H263PROFILETYPE {
    OMX_VIDEO_H263ProfileBaseline            = 0x01,        
    OMX_VIDEO_H263ProfileH320Coding          = 0x02,          
    OMX_VIDEO_H263ProfileBackwardCompatible  = 0x04,  
    OMX_VIDEO_H263ProfileISWV2               = 0x08,               
    OMX_VIDEO_H263ProfileISWV3               = 0x10,               
    OMX_VIDEO_H263ProfileHighCompression     = 0x20,     
    OMX_VIDEO_H263ProfileInternet            = 0x40,            
    OMX_VIDEO_H263ProfileInterlace           = 0x80,           
    OMX_VIDEO_H263ProfileHighLatency         = 0x100,         
    OMX_VIDEO_H263ProfileKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_VIDEO_H263ProfileVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_VIDEO_H263ProfileMax                 = 0x7FFFFFFF  
} OMX_VIDEO_H263PROFILETYPE;


typedef enum OMX_VIDEO_H263LEVELTYPE {
    OMX_VIDEO_H263Level10  = 0x01,  
    OMX_VIDEO_H263Level20  = 0x02,      
    OMX_VIDEO_H263Level30  = 0x04,      
    OMX_VIDEO_H263Level40  = 0x08,      
    OMX_VIDEO_H263Level45  = 0x10,      
    OMX_VIDEO_H263Level50  = 0x20,      
    OMX_VIDEO_H263Level60  = 0x40,      
    OMX_VIDEO_H263Level70  = 0x80, 
    OMX_VIDEO_H263LevelKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_VIDEO_H263LevelVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_VIDEO_H263LevelMax = 0x7FFFFFFF  
} OMX_VIDEO_H263LEVELTYPE;


typedef enum OMX_VIDEO_PICTURETYPE {
    OMX_VIDEO_PictureTypeI   = 0x01,
    OMX_VIDEO_PictureTypeP   = 0x02,
    OMX_VIDEO_PictureTypeB   = 0x04,
    OMX_VIDEO_PictureTypeSI  = 0x08,
    OMX_VIDEO_PictureTypeSP  = 0x10,
    OMX_VIDEO_PictureTypeEI  = 0x11,
    OMX_VIDEO_PictureTypeEP  = 0x12,
    OMX_VIDEO_PictureTypeS   = 0x14,
    OMX_VIDEO_PictureTypeKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_VIDEO_PictureTypeVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_VIDEO_PictureTypeMax = 0x7FFFFFFF
} OMX_VIDEO_PICTURETYPE;


typedef struct OMX_VIDEO_PARAM_H263TYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nPFrames;
    OMX_U32 nBFrames;
    OMX_VIDEO_H263PROFILETYPE eProfile;
	OMX_VIDEO_H263LEVELTYPE eLevel;
    OMX_BOOL bPLUSPTYPEAllowed;
    OMX_U32 nAllowedPictureTypes;
    OMX_BOOL bForceRoundingTypeToZero;
    OMX_U32 nPictureHeaderRepetition;
    OMX_U32 nGOBHeaderInterval;
} OMX_VIDEO_PARAM_H263TYPE;


typedef enum OMX_VIDEO_MPEG2PROFILETYPE {
    OMX_VIDEO_MPEG2ProfileSimple = 0,  /**< Simple Profile */
    OMX_VIDEO_MPEG2ProfileMain,        /**< Main Profile */
    OMX_VIDEO_MPEG2Profile422,         /**< 4:2:2 Profile */
    OMX_VIDEO_MPEG2ProfileSNR,         /**< SNR Profile */
    OMX_VIDEO_MPEG2ProfileSpatial,     /**< Spatial Profile */
    OMX_VIDEO_MPEG2ProfileHigh,        /**< High Profile */
    OMX_VIDEO_MPEG2ProfileKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_VIDEO_MPEG2ProfileVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_VIDEO_MPEG2ProfileMax = 0x7FFFFFFF  
} OMX_VIDEO_MPEG2PROFILETYPE;


typedef enum OMX_VIDEO_MPEG2LEVELTYPE {
    OMX_VIDEO_MPEG2LevelLL = 0,  /**< Low Level */ 
    OMX_VIDEO_MPEG2LevelML,      /**< Main Level */ 
    OMX_VIDEO_MPEG2LevelH14,     /**< High 1440 */ 
    OMX_VIDEO_MPEG2LevelHL,      /**< High Level */   
    OMX_VIDEO_MPEG2LevelKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_VIDEO_MPEG2LevelVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_VIDEO_MPEG2LevelMax = 0x7FFFFFFF  
} OMX_VIDEO_MPEG2LEVELTYPE;


typedef struct OMX_VIDEO_PARAM_MPEG2TYPE {
    OMX_U32 nSize;           
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;      
    OMX_U32 nPFrames;        
    OMX_U32 nBFrames;        
    OMX_VIDEO_MPEG2PROFILETYPE eProfile;
	OMX_VIDEO_MPEG2LEVELTYPE eLevel;   
} OMX_VIDEO_PARAM_MPEG2TYPE;


typedef enum OMX_VIDEO_MPEG4PROFILETYPE {
    OMX_VIDEO_MPEG4ProfileSimple           = 0x01,        
    OMX_VIDEO_MPEG4ProfileSimpleScalable   = 0x02,    
    OMX_VIDEO_MPEG4ProfileCore             = 0x04,              
    OMX_VIDEO_MPEG4ProfileMain             = 0x08,             
    OMX_VIDEO_MPEG4ProfileNbit             = 0x10,              
    OMX_VIDEO_MPEG4ProfileScalableTexture  = 0x20,   
    OMX_VIDEO_MPEG4ProfileSimpleFace       = 0x40,        
    OMX_VIDEO_MPEG4ProfileSimpleFBA        = 0x80,         
    OMX_VIDEO_MPEG4ProfileBasicAnimated    = 0x100,     
    OMX_VIDEO_MPEG4ProfileHybrid           = 0x200,            
    OMX_VIDEO_MPEG4ProfileAdvancedRealTime = 0x400,  
    OMX_VIDEO_MPEG4ProfileCoreScalable     = 0x800,      
    OMX_VIDEO_MPEG4ProfileAdvancedCoding   = 0x1000,    
    OMX_VIDEO_MPEG4ProfileAdvancedCore     = 0x2000,      
    OMX_VIDEO_MPEG4ProfileAdvancedScalable = 0x4000,
    OMX_VIDEO_MPEG4ProfileAdvancedSimple   = 0x8000,
    OMX_VIDEO_MPEG4ProfileKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_VIDEO_MPEG4ProfileVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_VIDEO_MPEG4ProfileMax              = 0x7FFFFFFF  
} OMX_VIDEO_MPEG4PROFILETYPE;


typedef enum OMX_VIDEO_MPEG4LEVELTYPE {
    OMX_VIDEO_MPEG4Level0  = 0x01,   /**< Level 0 */   
    OMX_VIDEO_MPEG4Level0b = 0x02,   /**< Level 0b */   
    OMX_VIDEO_MPEG4Level1  = 0x04,   /**< Level 1 */ 
    OMX_VIDEO_MPEG4Level2  = 0x08,   /**< Level 2 */ 
    OMX_VIDEO_MPEG4Level3  = 0x10,   /**< Level 3 */ 
    OMX_VIDEO_MPEG4Level4  = 0x20,   /**< Level 4 */  
    OMX_VIDEO_MPEG4Level4a = 0x40,   /**< Level 4a */  
    OMX_VIDEO_MPEG4Level5  = 0x80,   /**< Level 5 */  
    OMX_VIDEO_MPEG4LevelKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_VIDEO_MPEG4LevelVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_VIDEO_MPEG4LevelMax = 0x7FFFFFFF  
} OMX_VIDEO_MPEG4LEVELTYPE;


typedef struct OMX_VIDEO_PARAM_MPEG4TYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nSliceHeaderSpacing;
    OMX_BOOL bSVH;
    OMX_BOOL bGov;
    OMX_U32 nPFrames;
    OMX_U32 nBFrames;
    OMX_U32 nIDCVLCThreshold;
    OMX_BOOL bACPred;
    OMX_U32 nMaxPacketSize;
    OMX_U32 nTimeIncRes;
    OMX_VIDEO_MPEG4PROFILETYPE eProfile;
    OMX_VIDEO_MPEG4LEVELTYPE eLevel;
    OMX_U32 nAllowedPictureTypes;
    OMX_U32 nHeaderExtension;
    OMX_BOOL bReversibleVLC;
} OMX_VIDEO_PARAM_MPEG4TYPE;


typedef enum OMX_VIDEO_WMVFORMATTYPE {
    OMX_VIDEO_WMVFormatUnused = 0x01,   /**< Format unused or unknown */
    OMX_VIDEO_WMVFormat7      = 0x02,   /**< Windows Media Video format 7 */
    OMX_VIDEO_WMVFormat8      = 0x04,   /**< Windows Media Video format 8 */
    OMX_VIDEO_WMVFormat9      = 0x08,   /**< Windows Media Video format 9 */
    OMX_VIDEO_WMFFormatKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_VIDEO_WMFFormatVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_VIDEO_WMVFormatMax    = 0x7FFFFFFF
} OMX_VIDEO_WMVFORMATTYPE;


typedef struct OMX_VIDEO_PARAM_WMVTYPE {
    OMX_U32 nSize; 
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_VIDEO_WMVFORMATTYPE eFormat;
} OMX_VIDEO_PARAM_WMVTYPE;


typedef enum OMX_VIDEO_RVFORMATTYPE {
    OMX_VIDEO_RVFormatUnused = 0, /**< Format unused or unknown */
    OMX_VIDEO_RVFormat8,          /**< Real Video format 8 */
    OMX_VIDEO_RVFormat9,          /**< Real Video format 9 */
    OMX_VIDEO_RVFormatG2,         /**< Real Video Format G2 */
    OMX_VIDEO_RVFormatKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_VIDEO_RVFormatVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_VIDEO_RVFormatMax = 0x7FFFFFFF
} OMX_VIDEO_RVFORMATTYPE;


typedef struct OMX_VIDEO_PARAM_RVTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_VIDEO_RVFORMATTYPE eFormat;
    OMX_U16 nBitsPerPixel;
    OMX_U16 nPaddedWidth;
    OMX_U16 nPaddedHeight;
    OMX_U32 nFrameRate;
    OMX_U32 nBitstreamFlags;
    OMX_U32 nBitstreamVersion;
    OMX_U32 nMaxEncodeFrameSize;
    OMX_BOOL bEnablePostFilter;
    OMX_BOOL bEnableTemporalInterpolation;
    OMX_BOOL bEnableLatencyMode;
} OMX_VIDEO_PARAM_RVTYPE;


typedef enum OMX_VIDEO_AVCPROFILETYPE {
    OMX_VIDEO_AVCProfileBaseline = 0x01,   /**< Baseline profile */
    OMX_VIDEO_AVCProfileMain     = 0x02,   /**< Main profile */
    OMX_VIDEO_AVCProfileExtended = 0x04,   /**< Extended profile */
    OMX_VIDEO_AVCProfileHigh     = 0x08,   /**< High profile */
    OMX_VIDEO_AVCProfileHigh10   = 0x10,   /**< High 10 profile */
    OMX_VIDEO_AVCProfileHigh422  = 0x20,   /**< High 4:2:2 profile */
    OMX_VIDEO_AVCProfileHigh444  = 0x40,   /**< High 4:4:4 profile */
    OMX_VIDEO_AVCProfileKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_VIDEO_AVCProfileVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_VIDEO_AVCProfileMax      = 0x7FFFFFFF  
} OMX_VIDEO_AVCPROFILETYPE;


typedef enum OMX_VIDEO_AVCLEVELTYPE {
    OMX_VIDEO_AVCLevel1   = 0x01,     /**< Level 1 */
    OMX_VIDEO_AVCLevel1b  = 0x02,     /**< Level 1b */
    OMX_VIDEO_AVCLevel11  = 0x04,     /**< Level 1.1 */
    OMX_VIDEO_AVCLevel12  = 0x08,     /**< Level 1.2 */
    OMX_VIDEO_AVCLevel13  = 0x10,     /**< Level 1.3 */
    OMX_VIDEO_AVCLevel2   = 0x20,     /**< Level 2 */
    OMX_VIDEO_AVCLevel21  = 0x40,     /**< Level 2.1 */
    OMX_VIDEO_AVCLevel22  = 0x80,     /**< Level 2.2 */
    OMX_VIDEO_AVCLevel3   = 0x100,    /**< Level 3 */
    OMX_VIDEO_AVCLevel31  = 0x200,    /**< Level 3.1 */
    OMX_VIDEO_AVCLevel32  = 0x400,    /**< Level 3.2 */
    OMX_VIDEO_AVCLevel4   = 0x800,    /**< Level 4 */
    OMX_VIDEO_AVCLevel41  = 0x1000,   /**< Level 4.1 */
    OMX_VIDEO_AVCLevel42  = 0x2000,   /**< Level 4.2 */
    OMX_VIDEO_AVCLevel5   = 0x4000,   /**< Level 5 */
    OMX_VIDEO_AVCLevel51  = 0x8000,   /**< Level 5.1 */
    OMX_VIDEO_AVCLevelKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_VIDEO_AVCLevelVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_VIDEO_AVCLevelMax = 0x7FFFFFFF  
} OMX_VIDEO_AVCLEVELTYPE;


typedef enum OMX_VIDEO_AVCLOOPFILTERTYPE {
    OMX_VIDEO_AVCLoopFilterEnable = 0,
    OMX_VIDEO_AVCLoopFilterDisable,
    OMX_VIDEO_AVCLoopFilterDisableSliceBoundary,
    OMX_VIDEO_AVCLoopFilterKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_VIDEO_AVCLoopFilterVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_VIDEO_AVCLoopFilterMax = 0x7FFFFFFF
} OMX_VIDEO_AVCLOOPFILTERTYPE;


typedef struct OMX_VIDEO_PARAM_AVCTYPE {
    OMX_U32 nSize;                 
    OMX_VERSIONTYPE nVersion;      
    OMX_U32 nPortIndex;            
    OMX_U32 nSliceHeaderSpacing;  
    OMX_U32 nPFrames;     
    OMX_U32 nBFrames;     
    OMX_BOOL bUseHadamard;
    OMX_U32 nRefFrames;  
	OMX_U32 nRefIdx10ActiveMinus1;
	OMX_U32 nRefIdx11ActiveMinus1;
    OMX_BOOL bEnableUEP;  
    OMX_BOOL bEnableFMO;  
    OMX_BOOL bEnableASO;  
    OMX_BOOL bEnableRS;   
    OMX_VIDEO_AVCPROFILETYPE eProfile;
	OMX_VIDEO_AVCLEVELTYPE eLevel; 
    OMX_U32 nAllowedPictureTypes;  
	OMX_BOOL bFrameMBsOnly;        									
    OMX_BOOL bMBAFF;               
    OMX_BOOL bEntropyCodingCABAC;  
    OMX_BOOL bWeightedPPrediction; 
    OMX_U32 nWeightedBipredicitonMode; 
    OMX_BOOL bconstIpred ;
    OMX_BOOL bDirect8x8Inference;  
	OMX_BOOL bDirectSpatialTemporal;
	OMX_U32 nCabacInitIdc;
	OMX_VIDEO_AVCLOOPFILTERTYPE eLoopFilterMode;
} OMX_VIDEO_PARAM_AVCTYPE;

typedef struct OMX_VIDEO_PARAM_PROFILELEVELTYPE {
   OMX_U32 nSize;                 
   OMX_VERSIONTYPE nVersion;      
   OMX_U32 nPortIndex;            
   OMX_U32 eProfile;      /**< type is OMX_VIDEO_AVCPROFILETYPE, OMX_VIDEO_H263PROFILETYPE, 
                                 or OMX_VIDEO_MPEG4PROFILETYPE depending on context */
   OMX_U32 eLevel;        /**< type is OMX_VIDEO_AVCLEVELTYPE, OMX_VIDEO_H263LEVELTYPE, 
                                 or OMX_VIDEO_MPEG4PROFILETYPE depending on context */
   OMX_U32 nProfileIndex; /**< Used to query for individual profile support information,
                               This parameter is valid only for 
                               OMX_IndexParamVideoProfileLevelQuerySupported index,
                               For all other indices this parameter is to be ignored. */
} OMX_VIDEO_PARAM_PROFILELEVELTYPE;

typedef struct OMX_VIDEO_CONFIG_BITRATETYPE {
    OMX_U32 nSize;                          
    OMX_VERSIONTYPE nVersion;               
    OMX_U32 nPortIndex;                     
    OMX_U32 nEncodeBitrate;                 
} OMX_VIDEO_CONFIG_BITRATETYPE;

typedef struct OMX_CONFIG_FRAMERATETYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 xEncodeFramerate; /* Q16 format */
} OMX_CONFIG_FRAMERATETYPE;

typedef struct OMX_CONFIG_INTRAREFRESHVOPTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL IntraRefreshVOP;
} OMX_CONFIG_INTRAREFRESHVOPTYPE;

typedef struct OMX_CONFIG_MACROBLOCKERRORMAPTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nErrMapSize;           /* Size of the Error Map in bytes */
    OMX_U8  ErrMap[1];             /* Error map hint */
} OMX_CONFIG_MACROBLOCKERRORMAPTYPE;

typedef struct OMX_CONFIG_MBERRORREPORTINGTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bEnabled;
} OMX_CONFIG_MBERRORREPORTINGTYPE;

typedef struct OMX_PARAM_MACROBLOCKSTYPE {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nMacroblocks;
} OMX_PARAM_MACROBLOCKSTYPE;

typedef enum OMX_VIDEO_AVCSLICEMODETYPE {
    OMX_VIDEO_SLICEMODE_AVCDefault = 0,
    OMX_VIDEO_SLICEMODE_AVCMBSlice,
    OMX_VIDEO_SLICEMODE_AVCByteSlice,
    OMX_VIDEO_SLICEMODE_AVCKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_VIDEO_SLICEMODE_AVCVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_VIDEO_SLICEMODE_AVCLevelMax = 0x7FFFFFFF
} OMX_VIDEO_AVCSLICEMODETYPE;

typedef struct OMX_VIDEO_PARAM_AVCSLICEFMO {
    OMX_U32 nSize; 
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U8 nNumSliceGroups;
    OMX_U8 nSliceGroupMapType;
    OMX_VIDEO_AVCSLICEMODETYPE eSliceMode;
} OMX_VIDEO_PARAM_AVCSLICEFMO;

typedef struct OMX_VIDEO_CONFIG_AVCINTRAPERIOD {
    OMX_U32 nSize; 
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nIDRPeriod;
    OMX_U32 nPFrames;
} OMX_VIDEO_CONFIG_AVCINTRAPERIOD;

typedef struct OMX_VIDEO_CONFIG_NALSIZE {
    OMX_U32 nSize; 
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_U32 nNaluBytes;
} OMX_VIDEO_CONFIG_NALSIZE;

/** @} */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/* File EOF */

