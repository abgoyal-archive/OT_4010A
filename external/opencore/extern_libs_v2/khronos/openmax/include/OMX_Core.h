


#ifndef OMX_Core_h
#define OMX_Core_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



#include <OMX_Index.h>


typedef enum OMX_COMMANDTYPE
{
    OMX_CommandStateSet,    /**< Change the component state */
    OMX_CommandFlush,       /**< Flush the data queue(s) of a component */
    OMX_CommandPortDisable, /**< Disable a port on a component. */
    OMX_CommandPortEnable,  /**< Enable a port on a component. */
    OMX_CommandMarkBuffer,  /**< Mark a component/buffer for observation */
    OMX_CommandKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_CommandVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_CommandMax = 0X7FFFFFFF
} OMX_COMMANDTYPE;




typedef enum OMX_STATETYPE
{
    OMX_StateInvalid,      /**< component has detected that it's internal data 
                                structures are corrupted to the point that
                                it cannot determine it's state properly */
    OMX_StateLoaded,      /**< component has been loaded but has not completed
                                initialization.  The OMX_SetParameter macro
                                and the OMX_GetParameter macro are the only 
                                valid macros allowed to be sent to the 
                                component in this state. */
    OMX_StateIdle,        /**< component initialization has been completed
                                successfully and the component is ready to
                                to start. */
    OMX_StateExecuting,   /**< component has accepted the start command and
                                is processing data (if data is available) */
    OMX_StatePause,       /**< component has received pause command */
    OMX_StateWaitForResources, /**< component is waiting for resources, either after 
                                preemption or before it gets the resources requested.
                                See specification for complete details. */
    OMX_StateKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_StateVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_StateMax = 0X7FFFFFFF
} OMX_STATETYPE;

typedef enum OMX_ERRORTYPE
{
  OMX_ErrorNone = 0,

  /** There were insufficient resources to perform the requested operation */
  OMX_ErrorInsufficientResources = (OMX_S32) 0x80001000,

  /** There was an error, but the cause of the error could not be determined */
  OMX_ErrorUndefined = (OMX_S32) 0x80001001,

  /** The component name string was not valid */
  OMX_ErrorInvalidComponentName = (OMX_S32) 0x80001002,

  /** No component with the specified name string was found */
  OMX_ErrorComponentNotFound = (OMX_S32) 0x80001003,

  /** The component specified did not have a "OMX_ComponentInit" or
      "OMX_ComponentDeInit entry point */
  OMX_ErrorInvalidComponent = (OMX_S32) 0x80001004,

  /** One or more parameters were not valid */
  OMX_ErrorBadParameter = (OMX_S32) 0x80001005,

  /** The requested function is not implemented */
  OMX_ErrorNotImplemented = (OMX_S32) 0x80001006,

  /** The buffer was emptied before the next buffer was ready */
  OMX_ErrorUnderflow = (OMX_S32) 0x80001007,

  /** The buffer was not available when it was needed */
  OMX_ErrorOverflow = (OMX_S32) 0x80001008,

  /** The hardware failed to respond as expected */
  OMX_ErrorHardware = (OMX_S32) 0x80001009,

  /** The component is in the state OMX_StateInvalid */
  OMX_ErrorInvalidState = (OMX_S32) 0x8000100A,

  /** Stream is found to be corrupt */
  OMX_ErrorStreamCorrupt = (OMX_S32) 0x8000100B,

  /** Ports being connected are not compatible */
  OMX_ErrorPortsNotCompatible = (OMX_S32) 0x8000100C,

  /** Resources allocated to an idle component have been
      lost resulting in the component returning to the loaded state */
  OMX_ErrorResourcesLost = (OMX_S32) 0x8000100D,

  /** No more indicies can be enumerated */
  OMX_ErrorNoMore = (OMX_S32) 0x8000100E,

  /** The component detected a version mismatch */
  OMX_ErrorVersionMismatch = (OMX_S32) 0x8000100F,

  /** The component is not ready to return data at this time */
  OMX_ErrorNotReady = (OMX_S32) 0x80001010,

  /** There was a timeout that occurred */
  OMX_ErrorTimeout = (OMX_S32) 0x80001011,

  /** This error occurs when trying to transition into the state you are already in */
  OMX_ErrorSameState = (OMX_S32) 0x80001012,

  /** Resources allocated to an executing or paused component have been 
      preempted, causing the component to return to the idle state */
  OMX_ErrorResourcesPreempted = (OMX_S32) 0x80001013, 

  /** A non-supplier port sends this error to the IL client (via the EventHandler callback) 
      during the allocation of buffers (on a transition from the LOADED to the IDLE state or
      on a port restart) when it deems that it has waited an unusually long time for the supplier 
      to send it an allocated buffer via a UseBuffer call. */
  OMX_ErrorPortUnresponsiveDuringAllocation = (OMX_S32) 0x80001014,

  /** A non-supplier port sends this error to the IL client (via the EventHandler callback) 
      during the deallocation of buffers (on a transition from the IDLE to LOADED state or 
      on a port stop) when it deems that it has waited an unusually long time for the supplier 
      to request the deallocation of a buffer header via a FreeBuffer call. */
  OMX_ErrorPortUnresponsiveDuringDeallocation = (OMX_S32) 0x80001015,

  /** A supplier port sends this error to the IL client (via the EventHandler callback) 
      during the stopping of a port (either on a transition from the IDLE to LOADED 
      state or a port stop) when it deems that it has waited an unusually long time for 
      the non-supplier to return a buffer via an EmptyThisBuffer or FillThisBuffer call. */
  OMX_ErrorPortUnresponsiveDuringStop = (OMX_S32) 0x80001016,

  /** Attempting a state transtion that is not allowed */
  OMX_ErrorIncorrectStateTransition = (OMX_S32) 0x80001017,

  /* Attempting a command that is not allowed during the present state. */
  OMX_ErrorIncorrectStateOperation = (OMX_S32) 0x80001018, 

  /** The values encapsulated in the parameter or config structure are not supported. */
  OMX_ErrorUnsupportedSetting = (OMX_S32) 0x80001019,

  /** The parameter or config indicated by the given index is not supported. */
  OMX_ErrorUnsupportedIndex = (OMX_S32) 0x8000101A,

  /** The port index supplied is incorrect. */
  OMX_ErrorBadPortIndex = (OMX_S32) 0x8000101B,

  /** The port has lost one or more of its buffers and it thus unpopulated. */
  OMX_ErrorPortUnpopulated = (OMX_S32) 0x8000101C,

  /** Component suspended due to temporary loss of resources */
  OMX_ErrorComponentSuspended = (OMX_S32) 0x8000101D,

  /** Component suspended due to an inability to acquire dynamic resources */
  OMX_ErrorDynamicResourcesUnavailable = (OMX_S32) 0x8000101E,

  /** When the macroblock error reporting is enabled the component returns new error 
  for every frame that has errors */
  OMX_ErrorMbErrorsInFrame = (OMX_S32) 0x8000101F,

  /** A component reports this error when it cannot parse or determine the format of an input stream. */
  OMX_ErrorFormatNotDetected = (OMX_S32) 0x80001020, 

  /** The content open operation failed. */
  OMX_ErrorContentPipeOpenFailed = (OMX_S32) 0x80001021,

  /** The content creation operation failed. */
  OMX_ErrorContentPipeCreationFailed = (OMX_S32) 0x80001022,

  /** Separate table information is being used */
  OMX_ErrorSeperateTablesUsed = (OMX_S32) 0x80001023,

  /** Tunneling is unsupported by the component*/
  OMX_ErrorTunnelingUnsupported = (OMX_S32) 0x80001024,

  OMX_ErrorKhronosExtensions = (OMX_S32)0x8F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
  OMX_ErrorVendorStartUnused = (OMX_S32)0x90000000, /**< Reserved region for introducing Vendor Extensions */
  OMX_ErrorMax = 0x7FFFFFFF
} OMX_ERRORTYPE;

/** @ingroup core */
typedef OMX_ERRORTYPE (* OMX_COMPONENTINITTYPE)(OMX_IN  OMX_HANDLETYPE hComponent);

/** @ingroup core */
typedef struct OMX_COMPONENTREGISTERTYPE
{
  const char          * pName;       /* Component name, 128 byte limit (including '\0') applies */
  OMX_COMPONENTINITTYPE pInitialize; /* Component instance initialization function */
} OMX_COMPONENTREGISTERTYPE;

/** @ingroup core */
extern OMX_COMPONENTREGISTERTYPE OMX_ComponentRegistered[];

/** @ingroup rpm */
typedef struct OMX_PRIORITYMGMTTYPE {
 OMX_U32 nSize;             /**< size of the structure in bytes */
 OMX_VERSIONTYPE nVersion;  /**< OMX specification version information */
 OMX_U32 nGroupPriority;            /**< Priority of the component group */
 OMX_U32 nGroupID;                  /**< ID of the component group */
} OMX_PRIORITYMGMTTYPE;

/* Component name and Role names are limited to 128 characters including the terminating '\0'. */
#define OMX_MAX_STRINGNAME_SIZE 128

/** @ingroup comp */
typedef struct OMX_PARAM_COMPONENTROLETYPE {
    OMX_U32 nSize;              /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U8 cRole[OMX_MAX_STRINGNAME_SIZE];  /**< name of standard component which defines component role */
} OMX_PARAM_COMPONENTROLETYPE;


#define OMX_BUFFERFLAG_EOS 0x00000001 


#define OMX_BUFFERFLAG_STARTTIME 0x00000002

 


#define OMX_BUFFERFLAG_DECODEONLY 0x00000004



#define OMX_BUFFERFLAG_DATACORRUPT 0x00000008

#define OMX_BUFFERFLAG_ENDOFFRAME 0x00000010

#define OMX_BUFFERFLAG_SYNCFRAME 0x00000020

#define OMX_BUFFERFLAG_EXTRADATA 0x00000040

#define OMX_BUFFERFLAG_CODECCONFIG 0x00000080

//mtk80366 Remove start for alps00122003->

// Morris Yang 20100719 [
//#define OMX_BUFFERFLAG_LASTCODECCONFIG 0x00000100
// ]
//mtk80366 Remove end for alps00122003<-

// <--- Morris Yang 20110322 add for RV resizing
#define OMX_BUFFERFLAG_SPECIFYRESOLUTION 0x00000100
// --->

/** @ingroup buf */
typedef struct OMX_BUFFERHEADERTYPE
{
    OMX_U32 nSize;              /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U8* pBuffer;            /**< Pointer to actual block of memory 
                                     that is acting as the buffer */
    OMX_U32 nAllocLen;          /**< size of the buffer allocated, in bytes */
    OMX_U32 nFilledLen;         /**< number of bytes currently in the 
                                     buffer */
    OMX_U32 nOffset;            /**< start offset of valid data in bytes from
                                     the start of the buffer */
    OMX_PTR pAppPrivate;        /**< pointer to any data the application
                                     wants to associate with this buffer */
    OMX_PTR pPlatformPrivate;   /**< pointer to any data the platform
                                     wants to associate with this buffer */ 
    OMX_PTR pInputPortPrivate;  /**< pointer to any data the input port
                                     wants to associate with this buffer */
    OMX_PTR pOutputPortPrivate; /**< pointer to any data the output port
                                     wants to associate with this buffer */
    OMX_HANDLETYPE hMarkTargetComponent; /**< The component that will generate a 
                                              mark event upon processing this buffer. */
    OMX_PTR pMarkData;          /**< Application specific data associated with 
                                     the mark sent on a mark event to disambiguate 
                                     this mark from others. */
    OMX_U32 nTickCount;         /**< Optional entry that the component and
                                     application can update with a tick count
                                     when they access the component.  This
                                     value should be in microseconds.  Since
                                     this is a value relative to an arbitrary
                                     starting point, this value cannot be used 
                                     to determine absolute time.  This is an
                                     optional entry and not all components
                                     will update it.*/
 OMX_TICKS nTimeStamp;          /**< Timestamp corresponding to the sample 
                                     starting at the first logical sample 
                                     boundary in the buffer. Timestamps of 
                                     successive samples within the buffer may
                                     be inferred by adding the duration of the 
                                     of the preceding buffer to the timestamp
                                     of the preceding buffer.*/
  OMX_U32     nFlags;           /**< buffer specific flags */
  OMX_U32 nOutputPortIndex;     /**< The index of the output port (if any) using 
                                     this buffer */
  OMX_U32 nInputPortIndex;      /**< The index of the input port (if any) using
                                     this buffer */

// <--- Morris Yang 20110322 add for RV resizing
  OMX_U32 nWidth;
  OMX_U32 nHeight;
  OMX_U32 nStride;
  OMX_U32 nSliceHeight;
// --->

} OMX_BUFFERHEADERTYPE;

typedef enum OMX_EXTRADATATYPE
{
   OMX_ExtraDataNone = 0,                       /**< Indicates that no more extra data sections follow */        
   OMX_ExtraDataQuantization,                   /**< The data payload contains quantization data */
   OMX_ExtraDataKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
   OMX_ExtraDataVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
   OMX_ExtraDataMax = 0x7FFFFFFF
} OMX_EXTRADATATYPE;


typedef struct OMX_OTHER_EXTRADATATYPE  {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;               
    OMX_U32 nPortIndex;
    OMX_EXTRADATATYPE eType;       /* Extra Data type */
    OMX_U32 nDataSize;   /* Size of the supporting data to follow */
    OMX_U8  data[1];     /* Supporting data hint  */
} OMX_OTHER_EXTRADATATYPE;

/** @ingroup comp */
typedef struct OMX_PORT_PARAM_TYPE {
    OMX_U32 nSize;              /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion;   /**< OMX specification version information */
    OMX_U32 nPorts;             /**< The number of ports for this component */
    OMX_U32 nStartPortNumber;   /** first port number for this type of port */
} OMX_PORT_PARAM_TYPE; 

/** @ingroup comp */
typedef enum OMX_EVENTTYPE
{
    OMX_EventCmdComplete,         /**< component has sucessfully completed a command */
    OMX_EventError,               /**< component has detected an error condition */
    OMX_EventMark,                /**< component has detected a buffer mark */
    OMX_EventPortSettingsChanged, /**< component is reported a port settings change */
    OMX_EventBufferFlag,          /**< component has detected an EOS */ 
    OMX_EventResourcesAcquired,   /**< component has been granted resources and is
                                       automatically starting the state change from
                                       OMX_StateWaitForResources to OMX_StateIdle. */
   OMX_EventComponentResumed,     /**< Component resumed due to reacquisition of resources */
   OMX_EventDynamicResourcesAvailable, /**< Component has acquired previously unavailable dynamic resources */
   OMX_EventPortFormatDetected,      /**< Component has detected a supported format. */

// <--- Morris Yang 20110322 add for RV resizing
#ifndef ANDROID_DEFAULT_CODE
   OMX_EventPortResolutionChanged,
#endif
// --->

   OMX_EventKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
   OMX_EventVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
   OMX_EventMax = 0x7FFFFFFF
} OMX_EVENTTYPE;

typedef struct OMX_CALLBACKTYPE
{
    /** The EventHandler method is used to notify the application when an
        event of interest occurs.  Events are defined in the OMX_EVENTTYPE
        enumeration.  Please see that enumeration for details of what will
        be returned for each type of event. Callbacks should not return
        an error to the component, so if an error occurs, the application 
        shall handle it internally.  This is a blocking call.

        The application should return from this call within 5 msec to avoid
        blocking the component for an excessively long period of time.

        @param hComponent
            handle of the component to access.  This is the component
            handle returned by the call to the GetHandle function.
        @param pAppData
            pointer to an application defined value that was provided in the 
            pAppData parameter to the OMX_GetHandle method for the component.
            This application defined value is provided so that the application 
            can have a component specific context when receiving the callback.
        @param eEvent
            Event that the component wants to notify the application about.
        @param nData1
            nData will be the OMX_ERRORTYPE for an error event and will be 
            an OMX_COMMANDTYPE for a command complete event and OMX_INDEXTYPE for a OMX_PortSettingsChanged event.
         @param nData2
            nData2 will hold further information related to the event. Can be OMX_STATETYPE for
            a OMX_CommandStateSet command or port index for a OMX_PortSettingsChanged event.
            Default value is 0 if not used. )
        @param pEventData
            Pointer to additional event-specific data (see spec for meaning).
      */

   OMX_ERRORTYPE (*EventHandler)(
        OMX_IN OMX_HANDLETYPE hComponent,
        OMX_IN OMX_PTR pAppData,
        OMX_IN OMX_EVENTTYPE eEvent,
        OMX_IN OMX_U32 nData1,
        OMX_IN OMX_U32 nData2,
        OMX_IN OMX_PTR pEventData);

    /** The EmptyBufferDone method is used to return emptied buffers from an
        input port back to the application for reuse.  This is a blocking call 
        so the application should not attempt to refill the buffers during this
        call, but should queue them and refill them in another thread.  There
        is no error return, so the application shall handle any errors generated
        internally.  
        
        The application should return from this call within 5 msec.
        
        @param hComponent
            handle of the component to access.  This is the component
            handle returned by the call to the GetHandle function.
        @param pAppData
            pointer to an application defined value that was provided in the 
            pAppData parameter to the OMX_GetHandle method for the component.
            This application defined value is provided so that the application 
            can have a component specific context when receiving the callback.
        @param pBuffer
            pointer to an OMX_BUFFERHEADERTYPE structure allocated with UseBuffer
            or AllocateBuffer indicating the buffer that was emptied.
        @ingroup buf
     */
    OMX_ERRORTYPE (*EmptyBufferDone)(
        OMX_IN OMX_HANDLETYPE hComponent,
        OMX_IN OMX_PTR pAppData,
        OMX_IN OMX_BUFFERHEADERTYPE* pBuffer);

    /** The FillBufferDone method is used to return filled buffers from an
        output port back to the application for emptying and then reuse.  
        This is a blocking call so the application should not attempt to 
        empty the buffers during this call, but should queue the buffers 
        and empty them in another thread.  There is no error return, so 
        the application shall handle any errors generated internally.  The 
        application shall also update the buffer header to indicate the
        number of bytes placed into the buffer.  

        The application should return from this call within 5 msec.
        
        @param hComponent
            handle of the component to access.  This is the component
            handle returned by the call to the GetHandle function.
        @param pAppData
            pointer to an application defined value that was provided in the 
            pAppData parameter to the OMX_GetHandle method for the component.
            This application defined value is provided so that the application 
            can have a component specific context when receiving the callback.
        @param pBuffer
            pointer to an OMX_BUFFERHEADERTYPE structure allocated with UseBuffer
            or AllocateBuffer indicating the buffer that was filled.
        @ingroup buf
     */
    OMX_ERRORTYPE (*FillBufferDone)(
        OMX_OUT OMX_HANDLETYPE hComponent,
        OMX_OUT OMX_PTR pAppData,
        OMX_OUT OMX_BUFFERHEADERTYPE* pBuffer);

} OMX_CALLBACKTYPE;

typedef enum OMX_BUFFERSUPPLIERTYPE
{
    OMX_BufferSupplyUnspecified = 0x0, /**< port supplying the buffers is unspecified,
                                              or don't care */
    OMX_BufferSupplyInput,             /**< input port supplies the buffers */
    OMX_BufferSupplyOutput,            /**< output port supplies the buffers */
    OMX_BufferSupplyKhronosExtensions = 0x6F000000, /**< Reserved region for introducing Khronos Standard Extensions */ 
    OMX_BufferSupplyVendorStartUnused = 0x7F000000, /**< Reserved region for introducing Vendor Extensions */
    OMX_BufferSupplyMax = 0x7FFFFFFF
} OMX_BUFFERSUPPLIERTYPE;


typedef struct OMX_PARAM_BUFFERSUPPLIERTYPE {
    OMX_U32 nSize; /**< size of the structure in bytes */
    OMX_VERSIONTYPE nVersion; /**< OMX specification version information */
    OMX_U32 nPortIndex; /**< port that this structure applies to */
    OMX_BUFFERSUPPLIERTYPE eBufferSupplier; /**< buffer supplier */
} OMX_PARAM_BUFFERSUPPLIERTYPE;


#define OMX_PORTTUNNELFLAG_READONLY 0x00000001 


typedef struct OMX_TUNNELSETUPTYPE
{
    OMX_U32 nTunnelFlags;             /**< bit flags for tunneling */
    OMX_BUFFERSUPPLIERTYPE eSupplier; /**< supplier preference */
} OMX_TUNNELSETUPTYPE; 

/* TO BE REMOVED - #include <OMX_Component.h> */

#define OMX_GetComponentVersion(                            \
        hComponent,                                         \
        pComponentName,                                     \
        pComponentVersion,                                  \
        pSpecVersion,                                       \
        pComponentUUID)                                     \
    ((OMX_COMPONENTTYPE*)hComponent)->GetComponentVersion(  \
        hComponent,                                         \
        pComponentName,                                     \
        pComponentVersion,                                  \
        pSpecVersion,                                       \
        pComponentUUID)                 /* Macro End */


#define OMX_SendCommand(                                    \
         hComponent,                                        \
         Cmd,                                               \
         nParam,                                            \
         pCmdData)                                          \
     ((OMX_COMPONENTTYPE*)hComponent)->SendCommand(         \
         hComponent,                                        \
         Cmd,                                               \
         nParam,                                            \
         pCmdData)                          /* Macro End */


#define OMX_GetParameter(                                   \
        hComponent,                                         \
        nParamIndex,                                        \
        pComponentParameterStructure)                        \
    ((OMX_COMPONENTTYPE*)hComponent)->GetParameter(         \
        hComponent,                                         \
        nParamIndex,                                        \
        pComponentParameterStructure)    /* Macro End */


#define OMX_SetParameter(                                   \
        hComponent,                                         \
        nParamIndex,                                        \
        pComponentParameterStructure)                        \
    ((OMX_COMPONENTTYPE*)hComponent)->SetParameter(         \
        hComponent,                                         \
        nParamIndex,                                        \
        pComponentParameterStructure)    /* Macro End */


#define OMX_GetConfig(                                      \
        hComponent,                                         \
        nConfigIndex,                                       \
        pComponentConfigStructure)                           \
    ((OMX_COMPONENTTYPE*)hComponent)->GetConfig(            \
        hComponent,                                         \
        nConfigIndex,                                       \
        pComponentConfigStructure)       /* Macro End */


#define OMX_SetConfig(                                      \
        hComponent,                                         \
        nConfigIndex,                                       \
        pComponentConfigStructure)                           \
    ((OMX_COMPONENTTYPE*)hComponent)->SetConfig(            \
        hComponent,                                         \
        nConfigIndex,                                       \
        pComponentConfigStructure)       /* Macro End */


#define OMX_GetExtensionIndex(                              \
        hComponent,                                         \
        cParameterName,                                     \
        pIndexType)                                         \
    ((OMX_COMPONENTTYPE*)hComponent)->GetExtensionIndex(    \
        hComponent,                                         \
        cParameterName,                                     \
        pIndexType)                     /* Macro End */


#define OMX_GetState(                                       \
        hComponent,                                         \
        pState)                                             \
    ((OMX_COMPONENTTYPE*)hComponent)->GetState(             \
        hComponent,                                         \
        pState)                         /* Macro End */



#define OMX_UseBuffer(                                      \
           hComponent,                                      \
           ppBufferHdr,                                     \
           nPortIndex,                                      \
           pAppPrivate,                                     \
           nSizeBytes,                                      \
           pBuffer)                                         \
    ((OMX_COMPONENTTYPE*)hComponent)->UseBuffer(            \
           hComponent,                                      \
           ppBufferHdr,                                     \
           nPortIndex,                                      \
           pAppPrivate,                                     \
           nSizeBytes,                                      \
           pBuffer)


#define OMX_AllocateBuffer(                                 \
        hComponent,                                         \
        ppBuffer,                                           \
        nPortIndex,                                         \
        pAppPrivate,                                        \
        nSizeBytes)                                         \
    ((OMX_COMPONENTTYPE*)hComponent)->AllocateBuffer(       \
        hComponent,                                         \
        ppBuffer,                                           \
        nPortIndex,                                         \
        pAppPrivate,                                        \
        nSizeBytes)                     /* Macro End */


#define OMX_FreeBuffer(                                     \
        hComponent,                                         \
        nPortIndex,                                         \
        pBuffer)                                            \
    ((OMX_COMPONENTTYPE*)hComponent)->FreeBuffer(           \
        hComponent,                                         \
        nPortIndex,                                         \
        pBuffer)                        /* Macro End */


#define OMX_EmptyThisBuffer(                                \
        hComponent,                                         \
        pBuffer)                                            \
    ((OMX_COMPONENTTYPE*)hComponent)->EmptyThisBuffer(      \
        hComponent,                                         \
        pBuffer)                        /* Macro End */


#define OMX_FillThisBuffer(                                 \
        hComponent,                                         \
        pBuffer)                                            \
    ((OMX_COMPONENTTYPE*)hComponent)->FillThisBuffer(       \
        hComponent,                                         \
        pBuffer)                        /* Macro End */



#define OMX_UseEGLImage(                                    \
           hComponent,                                      \
           ppBufferHdr,                                     \
           nPortIndex,                                      \
           pAppPrivate,                                     \
           eglImage)                                        \
    ((OMX_COMPONENTTYPE*)hComponent)->UseEGLImage(          \
           hComponent,                                      \
           ppBufferHdr,                                     \
           nPortIndex,                                      \
           pAppPrivate,                                     \
           eglImage)

OMX_API OMX_ERRORTYPE OMX_APIENTRY OMX_Init(void);


OMX_API OMX_ERRORTYPE OMX_APIENTRY OMX_Deinit(void);


OMX_API OMX_ERRORTYPE OMX_APIENTRY OMX_ComponentNameEnum(
    OMX_OUT OMX_STRING cComponentName,
    OMX_IN  OMX_U32 nNameLength,
    OMX_IN  OMX_U32 nIndex);


OMX_API OMX_ERRORTYPE OMX_APIENTRY OMX_GetHandle(
    OMX_OUT OMX_HANDLETYPE* pHandle, 
    OMX_IN  OMX_STRING cComponentName,
    OMX_IN  OMX_PTR pAppData,
    OMX_IN  OMX_CALLBACKTYPE* pCallBacks);


OMX_API OMX_ERRORTYPE OMX_APIENTRY OMX_FreeHandle(
    OMX_IN  OMX_HANDLETYPE hComponent);



OMX_API OMX_ERRORTYPE OMX_APIENTRY OMX_SetupTunnel(
    OMX_IN  OMX_HANDLETYPE hOutput,
    OMX_IN  OMX_U32 nPortOutput,
    OMX_IN  OMX_HANDLETYPE hInput,
    OMX_IN  OMX_U32 nPortInput);
    
/** @ingroup cp */
OMX_API OMX_ERRORTYPE   OMX_GetContentPipe(
    OMX_OUT OMX_HANDLETYPE *hPipe,
    OMX_IN OMX_STRING szURI);

OMX_API OMX_ERRORTYPE OMX_GetComponentsOfRole ( 
	OMX_IN      OMX_STRING role,
    OMX_INOUT   OMX_U32 *pNumComps,
    OMX_INOUT   OMX_U8  **compNames);

OMX_API OMX_ERRORTYPE OMX_GetRolesOfComponent ( 
	OMX_IN      OMX_STRING compName, 
    OMX_INOUT   OMX_U32 *pNumRoles,
    OMX_OUT     OMX_U8 **roles);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
/* File EOF */

