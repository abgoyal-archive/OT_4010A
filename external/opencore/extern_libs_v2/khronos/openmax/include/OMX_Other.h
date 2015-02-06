


#ifndef OMX_Other_h
#define OMX_Other_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



#include <OMX_Core.h>


typedef enum OMX_OTHER_FORMATTYPE {
    OMX_OTHER_FormatTime = 0, /**< Transmission of various timestamps, elapsed time, 
                                   time deltas, etc */
    OMX_OTHER_FormatPower,    /**< Perhaps used for enabling/disabling power 
                                   management, setting clocks? */
    OMX_OTHER_FormatStats,    /**< Could be things such as frame rate, frames 
                                   dropped, etc */
    OMX_OTHER_FormatBinary,   /**< Arbitrary binary data */
    OMX_OTHER_FormatVendorReserved = 1000, /**< Starting value for vendor specific 
                                                formats */

    OMX_OTHER_FormatKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_OTHER_FormatVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_OTHER_FormatMax = 0x7FFFFFFF
} OMX_OTHER_FORMATTYPE;

typedef enum OMX_TIME_SEEKMODETYPE {
    OMX_TIME_SeekModeFast = 0, /**< Prefer seeking to an approximation
                                * of the requested seek position over   
                                * the actual seek position if it
                                * results in a faster seek. */
    OMX_TIME_SeekModeAccurate, /**< Prefer seeking to the actual seek 
                                * position over an approximation
                                * of the requested seek position even
                                * if it results in a slower seek. */
    OMX_TIME_SeekModeKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_TIME_SeekModeVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_TIME_SeekModeMax = 0x7FFFFFFF
} OMX_TIME_SEEKMODETYPE;

/* Structure representing the seekmode of the component */
typedef struct OMX_TIME_CONFIG_SEEKMODETYPE {
    OMX_U32 nSize;                  /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;       /**< OMX specification version information */
    OMX_TIME_SEEKMODETYPE eType;    /**< The seek mode */
} OMX_TIME_CONFIG_SEEKMODETYPE;

typedef struct OMX_TIME_CONFIG_TIMESTAMPTYPE {
    OMX_U32 nSize;               /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;    /**< OMX specification version
                                  *   information */
    OMX_U32 nPortIndex;     /**< port that this structure applies to */
    OMX_TICKS nTimestamp;  	     /**< timestamp .*/ 
} OMX_TIME_CONFIG_TIMESTAMPTYPE;  

/** Enumeration of possible reference clocks to the media time. */
typedef enum OMX_TIME_UPDATETYPE {
      OMX_TIME_UpdateRequestFulfillment,    /**< Update is the fulfillment of a media time request. */
      OMX_TIME_UpdateScaleChanged,	        /**< Update was generated because the scale chagned. */
      OMX_TIME_UpdateClockStateChanged,     /**< Update was generated because the clock state changed. */
      OMX_TIME_UpdateKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
      OMX_TIME_UpdateVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
      OMX_TIME_UpdateMax = 0x7FFFFFFF
} OMX_TIME_UPDATETYPE;

/** Enumeration of possible reference clocks to the media time. */
typedef enum OMX_TIME_REFCLOCKTYPE {
      OMX_TIME_RefClockNone,    /**< Use no references. */
      OMX_TIME_RefClockAudio,	/**< Use references sent through OMX_IndexConfigTimeCurrentAudioReference */
      OMX_TIME_RefClockVideo,   /**< Use references sent through OMX_IndexConfigTimeCurrentVideoReference */
      OMX_TIME_RefClockKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
      OMX_TIME_RefClockVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
      OMX_TIME_RefClockMax = 0x7FFFFFFF
} OMX_TIME_REFCLOCKTYPE;

/** Enumeration of clock states. */
typedef enum OMX_TIME_CLOCKSTATE {
      OMX_TIME_ClockStateRunning,             /**< Clock running. */
      OMX_TIME_ClockStateWaitingForStartTime, /**< Clock waiting until the 
                                               *   prescribed clients emit their
                                               *   start time. */
      OMX_TIME_ClockStateStopped,             /**< Clock stopped. */
      OMX_TIME_ClockStateKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
      OMX_TIME_ClockStateVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
      OMX_TIME_ClockStateMax = 0x7FFFFFFF
} OMX_TIME_CLOCKSTATE;


typedef struct OMX_TIME_CONFIG_MEDIATIMEREQUESTTYPE {
    OMX_U32 nSize;              /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPortIndex;         /**< port that this structure applies to */
    OMX_PTR pClientPrivate;     /**< Client private data to disabiguate this media time 
                                 *   from others (e.g. the number of the frame to deliver). 
                                 *   Duplicated in the media time structure that fulfills 
                                 *   this request. A value of zero is reserved for time scale 
                                 *   updates. */
    OMX_TICKS nMediaTimestamp;  /**< Media timestamp requested.*/ 
    OMX_TICKS nOffset;          /**< Amount of wall clock time by which this
                                 *   request should be fulfilled early */
} OMX_TIME_CONFIG_MEDIATIMEREQUESTTYPE;


typedef struct OMX_TIME_MEDIATIMETYPE {
    OMX_U32 nSize;                  /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;       /**< OMX specification version information */
    OMX_U32 nClientPrivate;         /**< Client private data to disabiguate this media time 
                                     *   from others. Copied from the media time request. 
                                     *   A value of zero is reserved for time scale updates. */
    OMX_TIME_UPDATETYPE eUpdateType; /**< Reason for the update */
    OMX_TICKS nMediaTimestamp;      /**< Media time requested. If no media time was 
                                     *   requested then this is the current media time. */ 
    OMX_TICKS nOffset;              /**< Amount of wall clock time by which this
                                     *   request was actually fulfilled early */

    OMX_TICKS nWallTimeAtMediaTime; /**< Wall time corresponding to nMediaTimeStamp.
                                     *   A client may compare this value to current
                                     *   media time obtained from the Clock Component to determine
                                     *   the wall time until the media timestamp is really
                                     *   current. */
    OMX_S32 xScale;                 /**< Current media time scale in Q16 format. */
    OMX_TIME_CLOCKSTATE eState;     /* Seeking Change. Added 7/12.*/
                                    /**< State of the media time. */
} OMX_TIME_MEDIATIMETYPE;  

typedef struct OMX_TIME_CONFIG_SCALETYPE {
    OMX_U32 nSize;                  /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;       /**< OMX specification version information */
    OMX_S32 xScale;                 /**< This is a value in Q16 format which is used for
                                     * scaling the media time */
} OMX_TIME_CONFIG_SCALETYPE;
 
/** Bits used to identify a clock port. Used in OMX_TIME_CONFIG_CLOCKSTATETYPE’s nWaitMask field */
#define OMX_CLOCKPORT0 0x00000001
#define OMX_CLOCKPORT1 0x00000002
#define OMX_CLOCKPORT2 0x00000004
#define OMX_CLOCKPORT3 0x00000008
#define OMX_CLOCKPORT4 0x00000010
#define OMX_CLOCKPORT5 0x00000020
#define OMX_CLOCKPORT6 0x00000040
#define OMX_CLOCKPORT7 0x00000080

typedef struct OMX_TIME_CONFIG_CLOCKSTATETYPE {
    OMX_U32 nSize;              /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version 
                                 *   information */
    OMX_TIME_CLOCKSTATE eState; /**< State of the media time. */
    OMX_TICKS nStartTime;       /**< Start time of the media time. */
    OMX_TICKS nOffset;          /**< Time to offset the media time by 
                                 * (e.g. preroll). Media time will be
                                 * reported to be nOffset ticks earlier.     
                                 */
    OMX_U32 nWaitMask;          /**< Mask of OMX_CLOCKPORT values. */
} OMX_TIME_CONFIG_CLOCKSTATETYPE;

typedef struct OMX_TIME_CONFIG_ACTIVEREFCLOCKTYPE {
    OMX_U32 nSize;                  /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;       /**< OMX specification version information */
    OMX_TIME_REFCLOCKTYPE eClock;   /**< Reference clock used to compute media time */                        
} OMX_TIME_CONFIG_ACTIVEREFCLOCKTYPE;

typedef struct OMX_OTHER_CONFIG_POWERTYPE {
    OMX_U32 nSize;            /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion; /**< OMX specification version information */
    OMX_BOOL bEnablePM;       /**< Flag to enable Power Management */
} OMX_OTHER_CONFIG_POWERTYPE;


typedef struct OMX_OTHER_CONFIG_STATSTYPE {
    OMX_U32 nSize;            /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion; /**< OMX specification version information */
    /* what goes here */
} OMX_OTHER_CONFIG_STATSTYPE;


typedef struct OMX_OTHER_PORTDEFINITIONTYPE {
    OMX_OTHER_FORMATTYPE eFormat;  /**< Type of data expected for this channel */
} OMX_OTHER_PORTDEFINITIONTYPE;

typedef struct OMX_OTHER_PARAM_PORTFORMATTYPE {
    OMX_U32 nSize; /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion; /**< OMX specification version information */
    OMX_U32 nPortIndex; /**< Indicates which port to set */
    OMX_U32 nIndex; /**< Indicates the enumeration index for the format from 0x0 to N-1 */
    OMX_OTHER_FORMATTYPE eFormat; /**< Type of data expected for this channel */
} OMX_OTHER_PARAM_PORTFORMATTYPE; 

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/* File EOF */
