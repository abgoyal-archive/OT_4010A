

//#define LOG_NDEBUG 0
#undef LOG_TAG
#define LOG_TAG "MetadataDriver"
#include <utils/Log.h>

#include <media/thread_init.h>
#include <core/SkBitmap.h>
#include <private/media/VideoFrame.h>

#include "metadatadriver.h"

#include "oscl_file_cfg.h"

#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)
#include "oscl_pmem.h"

#ifdef PMEM_NON_CACHED
#include "oscl_mm_queue.h"
#endif

// [Honda] use Keene's RR setting
#include <linux/rtpm_prio.h>
#undef LOG_TAG
#define LOG_TAG "MetadataDriver"


#define PMEM_ALLOC(va,pa,size) \
do { \
    va = (uint8 *)oscl_pmem_alloc(size); \
    if (va == NULL) LOGE("[MetadataDriver][ERROR] Alloc va = NULL!!, TID:%d", gettid()); \
    pa = (uint8 *)oscl_pmem_vir2phy(va); \
    LOGD("[MetadataDriver] Alloc va = 0x%08x, pa = 0x%08x, size = %d, TID:%d",(uint32)va,(uint32)pa,size,gettid()); \
} while (0)

#define PMEM_FREE(va) \
do { \
    oscl_pmem_free(va); \
    LOGD("[MetadataDriver] Free va = 0x%08x, TID:%d",(uint32)va,gettid()); \
} while (0)
#endif //#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)

#define METADATA_TRYALLOCMEMCOUNT    200

static bool bShowLog = false;
#define LOGP(...) \
{\
    if (bShowLog)\
    {\
        LOGD(__VA_ARGS__);\
    }\
    else\
    {\
        LOGV(__VA_ARGS__);\
    }\
}

#define SHOW_PROFILE 0
#if SHOW_PROFILE
#define LOG_PROFILE(...) LOGD(__VA_ARGS__)
#else
#define LOG_PROFILE(...) LOGV(__VA_ARGS__)
#endif

using namespace android;

const char* MetadataDriver::ALBUM_ART_KEY = "graphic";

const char* MetadataDriver::METADATA_KEYS[NUM_METADATA_KEYS] = {
        "track-info/track-number",
        "album",
        "artist",
        "author",
        "composer",
        "date",
        "genre",
        "title",
        "year",
        "duration",
        "num-tracks",
        "drm/is-protected",
        "track-info/codec-name",
        "rating",
        "comment",
        "copyright",
        "track-info/bit-rate",
        "track-info/frame-rate",
        "track-info/video/format",
        "track-info/video/height",
        "track-info/video/width",
        "writer",
};

static void dumpkeystolog(PVPMetadataList list)
{
    LOGP("dumpkeystolog");
    uint32 n = list.size();
    for(uint32 i = 0; i < n; ++i) {
        LOGI("@@@@@ wma key: %s", list[i].get_cstr());
    }
}

MetadataDriver::MetadataDriver(uint32 mode): OsclActiveObject(OsclActiveObject::EPriorityNominal, "MetadataDriver")
{
    bShowLog = Oscl_File_Cfg::LoadCfg(Oscl_File_Cfg::CFG_PLAYER, "log_metadatadriver");
    
    LOGP("constructor");
    mMode = mode;
    mUtil = NULL;
    mDataSource = NULL;
#if BEST_THUMBNAIL_MODE
    mLocalDataSource = NULL;
#endif
    mCmdId = 0;
    mContextObjectRefValue = 0x5C7A; // Some random number
    mContextObject = mContextObjectRefValue;
    mMediaAlbumArt = NULL;
    mSharedFd = -1;
    mVideoFrame = NULL;
    for (uint32 i = 0; i < NUM_METADATA_KEYS; ++i) {
        mMetadataValues[i][0] = '\0';
    }

    PmemDstBufVA = NULL;
    PmemDstBufPA = NULL;
    PmemDstBufSize = 0;
    
    LOGP("constructor: Mode (%d).", mMode);
}

/*static*/ int MetadataDriver::startDriverThread(void *cookie)
{
    LOGP("startDriverThread");
    MetadataDriver *driver = (MetadataDriver *)cookie;
    return driver->retrieverThread();
}

int MetadataDriver::retrieverThread()
{
    int error;
#ifdef MHAL_H_INCLUDED    
      LOGD("!!!!!**************** retrieverThread  *******************!!!!!");
    struct sched_param sched_p;
    sched_getparam(0, &sched_p);
    // [Honda] use Keene's RR setting
    //sched_p.sched_priority = 2;
    sched_p.sched_priority = RTPM_PRIO_PV_METARETR;

    if(0 != sched_setscheduler(0, SCHED_RR, &sched_p)) {
        LOGE("[%s] failed, errno: %d", __func__, errno);
    } else {
        sched_p.sched_priority = 0;
        sched_getparam(0, &sched_p);
        LOGD("!!!!!  (0x%x) sched_setscheduler ok, priority: %d", this, sched_p.sched_priority);
    }
    LOGD("!!!!!**************************************************!!!!!");
#endif    
    LOGP("retrieverThread");
    if (!InitializeForThread()) {
        LOGP("InitializeForThread fail");
        mSyncSem->Signal();
        return -1;
    }

    OMX_MasterInit();
    OsclScheduler::Init("PVAuthorEngineWrapper");
    mState = STATE_CREATE;
    AddToScheduler();
    RunIfNotReady();
    OsclExecScheduler *sched = OsclExecScheduler::Current();

#if 0
    sched->StartScheduler();
#else
        OSCL_TRY(error, sched->StartScheduler());
        OSCL_FIRST_CATCH_ANY(error,
                // Some AO did a leave, log it
                         LOGE("MetadataDriver AO did a leave, error=%d", error)
                );
#endif

    mSyncSem->Signal();  // Signal that doSetDataSource() is done.
    OsclScheduler::Cleanup();
    OMX_MasterDeinit();
    UninitializeForThread();
    return 0;
}

MetadataDriver::~MetadataDriver()
{
    LOGP("destructor");

    mCmdId = 0;
    delete mVideoFrame;
    mVideoFrame = NULL;
    delete mMediaAlbumArt;
    mMediaAlbumArt = NULL;
    delete mSyncSem;
    mSyncSem = NULL;

    closeSharedFdIfNecessary();
}

const char* MetadataDriver::extractMetadata(int keyCode)
{
    LOGP("extractMetadata");
    char *value = NULL;
    if (mMode & GET_METADATA_ONLY) {
        // Comparing int with unsigned int
        if (keyCode < 0 || keyCode >= (int) NUM_METADATA_KEYS) {
            LOGE("extractMetadata: Invalid keyCode: %d.", keyCode);
        } else {
            value = mMetadataValues[keyCode];
        }
    }
    if (value == NULL || value[0] == '\0') {
        return NULL;
    }
    return value;
}

MediaAlbumArt *MetadataDriver::extractAlbumArt()
{
    LOGP("extractAlbumArt");
    if (mMode & GET_METADATA_ONLY) {  // copy out
        if (mMediaAlbumArt != NULL && mMediaAlbumArt->mSize > 0) {
            return new MediaAlbumArt(*mMediaAlbumArt);
        } else {
            LOGE("failed to extract album art");
            return NULL;
        }
    }
    LOGE("extractAlbumArt: invalid mode (%d) to extract album art", mMode);
    return NULL;
}

// How to better manage these constant strings?
bool MetadataDriver::containsSupportedKey(const OSCL_HeapString<OsclMemAllocator>& str) const
{
    LOGP("containsSupportedKey");
    const char* cStr = str.get_cstr();
    for (uint32 i = 0; i < NUM_METADATA_KEYS; ++i) {
        if (strcasestr(cStr, METADATA_KEYS[i])) {
            return true;
        }
    }

    // Key "graphic" is a special metadata key for retrieving album art image.
    if (strcasestr(cStr, "graphic")) {
        return true;
    }
    return false;
}

// Delete unnecessary keys before retrieving the metadata values to avoid
// retrieving all metadata values for all metadata keys
void MetadataDriver::trimKeys()
{
    LOGP("trimKeys");
    //dumpkeystolog(mMetadataKeyList);
    mActualMetadataKeyList.clear();
    uint32 n = mMetadataKeyList.size();
    mActualMetadataKeyList.reserve(n);
    for (uint32 i = 0; i < n; ++i) {
        if (containsSupportedKey(mMetadataKeyList[i])) {
            mActualMetadataKeyList.push_back(mMetadataKeyList[i]);
        }
    }
    mMetadataKeyList.clear();
}

// Returns:
// 1. UNKNOWN_ERROR
//    a. If the metadata value(s) is too long, and cannot be hold in valueLength bytes
//    b. If nothing is found
// 2. OK
//    a. If metadata value(s) is found
status_t MetadataDriver::extractMetadata(const char* key, char* value, uint32 valueLength)
{
    LOGP("extractMetadata");
    bool found = false;
    value[0] = '\0';
    for (uint32 i = 0, n = mMetadataValueList.size(); i < n; ++i) {
        if (0 == strncasecmp(mMetadataValueList[i].key, key, strlen(key))) {
            found = true;
            switch(GetValTypeFromKeyString(mMetadataValueList[i].key)) {
                case PVMI_KVPVALTYPE_CHARPTR: {
                    uint32 length = oscl_strlen(mMetadataValueList[i].value.pChar_value) + 1;
                    if (length > valueLength) {
                        return UNKNOWN_ERROR;
                    }
                    oscl_snprintf(value, length, "%s", mMetadataValueList[i].value.pChar_value);
                    break;
                }
                case PVMI_KVPVALTYPE_WCHARPTR: {
                    // Assume string is in UCS-2 encoding so convert to UTF-8.
                    uint32 length = oscl_strlen(mMetadataValueList[i].value.pWChar_value) + 1;
                    if (length > valueLength) {
                        return UNKNOWN_ERROR;
                    }
                    length = oscl_UnicodeToUTF8(mMetadataValueList[i].value.pWChar_value, length, value, valueLength);
                    break;
                }
                case PVMI_KVPVALTYPE_UINT32: {
                    // FIXME:
                    // This is an ugly hack since OpenCore returns duration in the following two formats:
                    // 1. duration;valtype=uint32 (the duration is an integer representing milliseconds)
                    // 2. duration;valtype=uint32;timescale=8000 (the duration is an integer representing the
                    //    duration in a timescale of 8 kHz)
                    // It would be nice to have OpenCore always return duration in the first format.
                    // PV will study on fixing this to always return duration in the first format.
                    // Until that fix is available, we still need to do this.
                    const char* durKeyStr = "duration";
                    const char* timeScaleStr = "timescale=";
                    int timescale = 1000;
                    if (strncasecmp(key, durKeyStr, strlen(durKeyStr)) == 0) {
                        char *p;
                        if ((p = strcasestr(mMetadataValueList[i].key, timeScaleStr)) != NULL) {
                            timescale = atoi(p + strlen(timeScaleStr));
                        }
                    }
                    //Tina note: if key isn't duration, such as num-tracks
                    //value = num-tracks*1000/1000 = num-tracks;
                    //it's ok!
                    int data = (mMetadataValueList[i].value.uint32_value * 1000LL) / timescale;
                    LOGP("MetadataDriver::extractMetadata case PVMI_KVPVALTYPE_UINT32, timescale=%d, data=%d",timescale, data);
                    oscl_snprintf(value, valueLength, "%d", data);
                    break;
                }
                case PVMI_KVPVALTYPE_INT32:
                    oscl_snprintf(value, valueLength, "%d", mMetadataValueList[i].value.int32_value);
                    break;

                case PVMI_KVPVALTYPE_UINT8:
                    oscl_snprintf(value, valueLength, "%d", mMetadataValueList[i].value.uint8_value);
                    break;

                case PVMI_KVPVALTYPE_FLOAT:
                    oscl_snprintf(value, valueLength, "%f", mMetadataValueList[i].value.float_value);
                    break;

                case PVMI_KVPVALTYPE_DOUBLE:
                    oscl_snprintf(value, valueLength, "%f", mMetadataValueList[i].value.double_value);
                    break;

                case PVMI_KVPVALTYPE_BOOL:
                    oscl_snprintf(value, valueLength, "%s", mMetadataValueList[i].value.bool_value? "true": "false");
                    break;

                default:
                    return UNKNOWN_ERROR;
            }

            LOGP("value is: %s.", value);

            break;
        }
    }
    return found? OK: UNKNOWN_ERROR;
}

void MetadataDriver::cacheMetadataRetrievalResults()
{
    LOGP("cacheMetadataRetrievalResults");
#if _METADATA_DRIVER_INTERNAL_DEBUG_ENABLE_
    for (uint32 i = 0, n = mMetadataValueList.size(); i < n; ++i) {
        LOGP("Value %d:   Key string: %s.", (i+1), mMetadataValueList[i].key);
    }
#endif

    // adjust bit-rate value - ALPS00130264
    int n = (int)mMetadataValueList.size();

    const char* keyAudio = "track-info/bit-rate;valtype=uint32;index=0";
   	const char* keyVideo = "track-info/bit-rate;valtype=uint32;index=1";
   	int iAudio = -1;		// audio bit-rate index
   	int iVideo = -1;		// video bit-rate index

    for (int i = 0; i < n; ++i)
    {
       LOGP("mMetadataValueList[%d], key: %s", i, mMetadataValueList[i].key);
    	if (strncasecmp(mMetadataValueList[i].key, keyAudio,strlen(keyAudio))==0)
    		iAudio = i;
    	if (strncasecmp(mMetadataValueList[i].key, keyVideo,strlen(keyVideo))==0)
    		iVideo = i;
    }

    if (iAudio != -1 && iVideo != -1)
    {
    	int bitrate = mMetadataValueList[iAudio].value.int32_value
    			+ mMetadataValueList[iVideo].value.int32_value;
    	mMetadataValueList[iAudio].value.int32_value = bitrate;
    	mMetadataValueList[iVideo].value.int32_value = bitrate;
    }

    for (uint32 i = 0; i < NUM_METADATA_KEYS; ++i) {
        LOGP("extract metadata key: %s", METADATA_KEYS[i]);
        extractMetadata(METADATA_KEYS[i], mMetadataValues[i], MAX_METADATA_STRING_LENGTH - 1);
    }
    doExtractAlbumArt();
}

status_t MetadataDriver::extractEmbeddedAlbumArt(const PvmfApicStruct* apic)
{
    LOGP("extractEmbeddedAlbumArt");
#if (0)
// Honda: temp solution for ALPS00005911 and ALPS00005896
// Need detail study AAC ID3 tag parser (apic)   
    char* buf  = (char*) apic->iGraphicData;
    uint32 size = apic->iGraphicDataLen;
    LOGP("extractEmbeddedAlbumArt: Embedded graphic or album art (%d bytes) is found.", size);
    if (size && buf) {
        delete mMediaAlbumArt;
        mMediaAlbumArt = new MediaAlbumArt();
        if (mMediaAlbumArt == NULL) {
            LOGE("extractEmbeddedAlbumArt: Not enough memory to hold a MediaAlbumArt object");
            return NO_MEMORY;
        }
        mMediaAlbumArt->mSize = size;
        mMediaAlbumArt->mData = new uint8[size];
        if (mMediaAlbumArt->mData == NULL) {
            LOGE("extractEmbeddedAlbumArt: Not enough memory to hold the binary data of a MediaAlbumArt object");
            delete mMediaAlbumArt;
            mMediaAlbumArt = NULL;
            return NO_MEMORY;
        }
        memcpy(mMediaAlbumArt->mData, buf, size);
        return NO_ERROR;
    }
#endif
    return BAD_VALUE;
}

status_t MetadataDriver::extractExternalAlbumArt(const char* url)
{
    LOGP("extractExternalAlbumArt: External graphic or album art is found: %s.", url);
    delete mMediaAlbumArt;
    mMediaAlbumArt = new MediaAlbumArt(url);
    return (mMediaAlbumArt && mMediaAlbumArt->mSize > 0)? OK: BAD_VALUE; 
}

// Finds the first album art and extract it.
status_t MetadataDriver::doExtractAlbumArt()
{
    LOGP("doExtractAlbumArt");
    status_t status = UNKNOWN_ERROR;
    for (uint32 i = 0, n = mMetadataValueList.size(); i < n; ++i) {
        if (strcasestr(mMetadataValueList[i].key, ALBUM_ART_KEY)) {
            LOGP("doExtractAlbumArt: album art key: %s", mMetadataValueList[i].key);
            if (PVMI_KVPVALTYPE_KSV == GetValTypeFromKeyString(mMetadataValueList[i].key)) {
                const char* embeddedKey = "graphic;format=APIC;valtype=ksv";
                const char* externalKey = "graphic;valtype=char*";
                if (strstr(mMetadataValueList[i].key, embeddedKey) && mMetadataValueList[i].value.key_specific_value) {
                    // Embedded album art.
                    status = extractEmbeddedAlbumArt(((PvmfApicStruct*)mMetadataValueList[i].value.key_specific_value));
                } else if (strstr(mMetadataValueList[i].key, externalKey)) {
                    // Album art linked with an external url.
                    status = extractExternalAlbumArt(mMetadataValueList[i].value.pChar_value);
                }

                if (status != OK) {
                    continue;
                }
                return status;  // Found the album art.
            }
        }
    }
    return UNKNOWN_ERROR;
}

void MetadataDriver::clearCache()
{
    LOGP("clearCache");
    delete mVideoFrame;
    mVideoFrame = NULL;
    delete mMediaAlbumArt;
    mMediaAlbumArt = NULL;
    for(uint32 i = 0; i < NUM_METADATA_KEYS; ++i) {
        mMetadataValues[i][0] = '\0';
    }
}

status_t MetadataDriver::setDataSourceFd(
        int fd, int64_t offset, int64_t length) {
    LOGP("setDataSourceFd");

    closeSharedFdIfNecessary();

    if (offset < 0 || length < 0) {
        if (offset < 0) {
            LOGE("negative offset (%lld)", offset);
        }
        if (length < 0) {
            LOGE("negative length (%lld)", length);
        }
        return INVALID_OPERATION;
    }

    mSharedFd = dup(fd);

    char url[80];
    sprintf(url, "sharedfd://%d:%lld:%lld", mSharedFd, offset, length);

    clearCache();
    return doSetDataSource(url);
}

status_t MetadataDriver::setDataSource(const char* srcUrl)
{
    LOGP("setDataSource");

    closeSharedFdIfNecessary();

    // Don't let somebody trick us in to reading some random block of memory.
    if (strncmp("sharedfd://", srcUrl, 11) == 0) {
        LOGE("setDataSource: Invalid url (%s).", srcUrl);
        return UNKNOWN_ERROR;
    }

    if (oscl_strlen(srcUrl) > MAX_STRING_LENGTH) {
        LOGE("setDataSource: Data source url length (%d) is too long.", oscl_strlen(srcUrl));
        return UNKNOWN_ERROR;
    }
    clearCache();
    return doSetDataSource(srcUrl);
}

status_t MetadataDriver::doSetDataSource(const char* dataSrcUrl)
{
    LOGP("doSetDataSource");
    if (mMode & GET_FRAME_ONLY) {
#if BEST_THUMBNAIL_MODE
        mFrameSelector.iSelectionMethod = PVFrameSelector::SPECIFIC_FRAME;
        mFrameSelector.iFrameInfo.iTimeOffsetMilliSec = 0;
#else
        mFrameSelector.iSelectionMethod=PVFrameSelector::SPECIFIC_FRAME;
        mFrameSelector.iFrameInfo.iFrameIndex=0;
#endif
    }
    mIsSetDataSourceSuccessful = false;
    oscl_wchar tmpWCharBuf[MAX_STRING_LENGTH];
    oscl_UTF8ToUnicode(dataSrcUrl, oscl_strlen(dataSrcUrl), tmpWCharBuf, sizeof(tmpWCharBuf));
    mDataSourceUrl.set(tmpWCharBuf, oscl_strlen(tmpWCharBuf));
    mSyncSem = new OsclSemaphore();
    mSyncSem->Create();
    createThreadEtc(MetadataDriver::startDriverThread, this, "PVMetadataRetriever");
    mSyncSem->Wait();
    return mIsSetDataSourceSuccessful? OK: UNKNOWN_ERROR;
}

VideoFrame* MetadataDriver::captureFrame()
{
    LOGP("captureFrame");
    if (mMode & GET_FRAME_ONLY) {  // copy out
        if (mVideoFrame != NULL && mVideoFrame->mSize > 0) {
            return new VideoFrame(*mVideoFrame);
        } else {
            LOGE("failed to capture frame");
        // <--- Morris Yang ALPS00007068
        // In case of failure to capture the key frame, put black image (RGB565) as thumbnail
        #if 1
            VideoFrame _dummy_frame;
            _dummy_frame.mWidth = 92;
            _dummy_frame.mHeight = 92;
            _dummy_frame.mDisplayWidth = 92;
            _dummy_frame.mDisplayHeight = 92;
            _dummy_frame.mSize = 92*92*2;
            _dummy_frame.mData = new uint8[_dummy_frame.mSize];
            memset(_dummy_frame.mData, 0x00, _dummy_frame.mSize);
            return new VideoFrame(_dummy_frame);
        #else
        // --->
            return NULL;
        #endif
        }
    }
    LOGE("captureFrame: invalid mode (%d) to capture a frame", mMode);
    return NULL;
}

void MetadataDriver::doColorConversion()
{
    LOGP("doColorConversion");
    // Do color conversion using PV's color conversion utility
    int width  = mFrameBufferProp.iFrameWidth;
    int height = mFrameBufferProp.iFrameHeight;
    int displayWidth  = mFrameBufferProp.iDisplayWidth;
    int displayHeight = mFrameBufferProp.iDisplayHeight;
    int cropWidth = mFrameBufferProp.iCropWidth;
    int cropHeight = mFrameBufferProp.iCropHeight;
    uint32 PmemSrcBufVA = mFrameBufferProp.iPmemSrcBufVA;
    uint32 u4TryAllocMemCount;

    LOGP("width: %d, height: %d", width, height);
    LOGP("displayWidth: %d, displayHeight: %d", displayWidth, displayHeight);
    LOGP("PmemSrcBufVA = 0x%08x", PmemSrcBufVA);

#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)
#else
    SkBitmap *bitmap = new SkBitmap();
    if (!bitmap) {
        LOGE("doColorConversion: cannot instantiate a SkBitmap object.");
        return;
    }
#endif

    // Ryan Lin !!!
#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)

#if 0
    bitmap->setConfig(SkBitmap::kRGB_565_Config, displayWidth, displayHeight);
#else
    PmemDstBufSize = displayWidth * displayHeight * 2;

    u4TryAllocMemCount = METADATA_TRYALLOCMEMCOUNT;
    PmemDstBufVA = NULL;

    if (PmemDstBufSize == 0)
    {
        LOGE("[ERROR] PmemDstBufSize = 0, displayWidth = %d, displayHeight = %d", displayWidth, displayHeight);
        return;
    }
        
    while (u4TryAllocMemCount)
    {
        PMEM_ALLOC(PmemDstBufVA, PmemDstBufPA, PmemDstBufSize);

        if (PmemDstBufVA == NULL)
        {
            LOGE("Alloc PmemDstBufVA fail %d times!!, Try alloc again!!", (METADATA_TRYALLOCMEMCOUNT-u4TryAllocMemCount));
            u4TryAllocMemCount--;
            usleep(10*1000);
        }
        else
        {
            LOGD("Alloc PmemDstBufVA 0x%08x Success", (uint32)PmemDstBufVA);
            break;
        }
    }

    if (PmemDstBufVA == NULL)
    {
        LOGE("Alloc PmemDstBufVA fail %d times!!, Return error!!", METADATA_TRYALLOCMEMCOUNT);
        return;
    }    

#ifdef PMEM_NON_CACHED
    oscl_pmem_map_into_noncached((uint8*)PmemDstBufVA);
#endif    
    
#endif

#else

#ifndef MT6516_VIDEO_DECODER_OUTPUT_FORMAT    
    bitmap->setConfig(SkBitmap::kRGB_565_Config, displayWidth, displayHeight);
#else
    bitmap->setConfig(SkBitmap::kRGB_565_Config, width, height);
#endif

    if (!bitmap->allocPixels()) {
        LOGE("allocPixels failed");
        delete bitmap;
        return;
    }
#endif

    // When passing parameters in Init call of ColorConverter library
    // we need to take care of passing even width and height to the CC.
    // If Width is odd then making it even as below
    // will reduce the pitch by 1. This may result in a tilted image for
    // clips with odd width.

    // Ryan Lin !!!
#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)
    ColorConvertBase* colorConverter = ColorConvert16::NewL();
    ColorConvertBase::PixelFormat _pixelFormat = ColorConvertBase::PIXEL_FORMAT_YUV_420_PLANER;
    LOGD("!!!!!**************************************************!!!!!");
    if (mFrameBufferProp.mVideoSubFormat == PVMF_MIME_YUV420_PLANAR_MTKYUV)
    {
        LOGD("!!!!!          Output Color Format: MTK-YUV            !!!!!");
        _pixelFormat = ColorConvertBase::PIXEL_FORMAT_YUV_420_PLANER_MTK;
    }
    else if (mFrameBufferProp.mVideoSubFormat == PVMF_MIME_YUV420_PLANAR)
    {
        LOGD("!!!!!          Output Color Format: YUV-420            !!!!!");
        _pixelFormat = ColorConvertBase::PIXEL_FORMAT_YUV_420_PLANER;
    }
    else
    {
        LOGD("!!!!!          Output Color Format: YUV-420            !!!!!");
        _pixelFormat = ColorConvertBase::PIXEL_FORMAT_YUV_420_PLANER;
    }
    LOGD("!!!!!**************************************************!!!!!");
    colorConverter->SetInputBufFormat(_pixelFormat);
    colorConverter->SetCropSize(cropWidth, cropHeight);
    // [20100423] Jackal
    colorConverter->SetPmemSrcBufVA(PmemSrcBufVA);
    colorConverter->SetPmemDstBufPA((uint32)PmemDstBufPA);
    if (!colorConverter ||
       !colorConverter->Init(((width)&(~1)), ((height)&(~1)), ((width)&(~1)), displayWidth, ((displayHeight)&(~1)), ((displayWidth)&(~1)), CCROTATE_NONE) ||
        //!colorConverter->Init(width, height, width, displayWidth, displayHeight, displayWidth, CCROTATE_NONE) ||
        !colorConverter->SetMode(1) ||
#if 0
        !colorConverter->Convert(mFrameBuffer, (uint8*)bitmap->getPixels())) {
#else        
        !colorConverter->Convert(mFrameBuffer, PmemDstBufVA)) {
#endif        
        LOGE("failed to do color conversion");
        delete colorConverter;
#if 0
        delete bitmap;
#endif
        return;
    }
    delete colorConverter;
#else
    
#ifndef MT6516_VIDEO_DECODER_OUTPUT_FORMAT 
   
    ColorConvertBase* colorConverter = ColorConvert16::NewL();
    colorConverter->SetCropSize(cropWidth, cropHeight);
    // [20100423] Jackal
    colorConverter->SetPmemSrcBufVA(PmemSrcBufVA);
    colorConverter->SetPmemDstBufPA((uint32)PmemDstBufPA);
    if (!colorConverter ||
       !colorConverter->Init(((width)&(~1)), ((height)&(~1)), ((width)&(~1)), displayWidth, ((displayHeight)&(~1)), ((displayWidth)&(~1)), CCROTATE_NONE) ||
        //!colorConverter->Init(width, height, width, displayWidth, displayHeight, displayWidth, CCROTATE_NONE) ||
        !colorConverter->SetMode(1) ||
        !colorConverter->Convert(mFrameBuffer, (uint8*)bitmap->getPixels())) 
    {
        LOGE("failed to do color conversion");
        delete colorConverter;
        delete bitmap;
        return;
    }
    delete colorConverter;
#else
    memcpy((uint8*)bitmap->getPixels(), mFrameBuffer, bitmap->getSize());
#endif

#endif

    // Store the SkBitmap pixels in a private shared structure with known
    // internal memory layout so that the pixels can be sent across the
    // binder interface
    delete mVideoFrame;
    mVideoFrame = new VideoFrame();
    if (!mVideoFrame) {
        LOGE("failed to allocate memory for a VideoFrame object");

#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)
#else
        delete bitmap;
#endif
        return;
    }
    mVideoFrame->mWidth = width;
    mVideoFrame->mHeight = height;
    mVideoFrame->mDisplayWidth  = displayWidth;
    mVideoFrame->mDisplayHeight = displayHeight;

#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)
    mVideoFrame->mSize = PmemDstBufSize;
#else
    mVideoFrame->mSize = bitmap->getSize();
#endif

    LOGP("display width (%d) and height (%d), and size (%d)", displayWidth, displayHeight, mVideoFrame->mSize);
    mVideoFrame->mData = new uint8[mVideoFrame->mSize];
    if (!mVideoFrame->mData) {
        LOGE("doColorConversion: cannot allocate buffer to hold SkBitmap pixels");

#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)
#else
        delete bitmap;
#endif

        delete mVideoFrame;
        mVideoFrame = NULL;
        return;
    }

#if defined(MT6516_MP4_HW_DECODER_V2) || defined(MT6573_MFV_HW)
    memcpy(mVideoFrame->mData, PmemDstBufVA, mVideoFrame->mSize);

    if (PmemDstBufVA != 0)
    {
        PMEM_FREE(PmemDstBufVA);
        PmemDstBufVA = NULL;
        PmemDstBufPA = NULL;
        PmemDstBufSize = 0;
    }
#else
    memcpy(mVideoFrame->mData, (uint8*) bitmap->getPixels(), mVideoFrame->mSize);
    delete bitmap;
#endif
}

// Instantiate a frame and metadata utility object.
void MetadataDriver::handleCreate()
{
    LOGP("handleCreate");
    int error = 0;
    OSCL_TRY(error, mUtil = PVFrameAndMetadataFactory::CreateFrameAndMetadataUtility((char*)PVMF_MIME_YUV420, this, this, this, false));
    if (error || mUtil->SetMode(PV_FRAME_METADATA_INTERFACE_MODE_SOURCE_METADATA_AND_THUMBNAIL) != PVMFSuccess) {
        handleCommandFailure();
    } else {
        mState = STATE_ADD_DATA_SOURCE;
        RunIfNotReady();
    }
}

// Create a data source and add it.
void MetadataDriver::handleAddDataSource()
{
    LOGP("handleAddDataSource");
    int error = 0;
    mDataSource = new PVPlayerDataSourceURL;
    if (mDataSource) {
        mDataSource->SetDataSourceURL(mDataSourceUrl);
        mDataSource->SetDataSourceFormatType((char*)PVMF_MIME_FORMAT_UNKNOWN);
        if (mMode & GET_FRAME_ONLY) {
#if BEST_THUMBNAIL_MODE
            // Set the intent to thumbnails.
            mLocalDataSource = new PVMFLocalDataSource();
            mLocalDataSource->iIntent = BITMASK_PVMF_SOURCE_INTENT_THUMBNAILS;
            mDataSource->SetDataSourceContextData((OsclAny*)mLocalDataSource);
#endif
        }
        OSCL_TRY(error, mCmdId = mUtil->AddDataSource(*mDataSource, (OsclAny*)&mContextObject));
        OSCL_FIRST_CATCH_ANY(error, handleCommandFailure());
    }
}

void MetadataDriver::handleRemoveDataSource()
{
    LOGP("handleRemoveDataSource");
    int error = 0;
    OSCL_TRY(error, mCmdId = mUtil->RemoveDataSource(*mDataSource, (OsclAny*)&mContextObject));
    OSCL_FIRST_CATCH_ANY(error, handleCommandFailure());
}

// Clean up, due to either failure or task completion.
void MetadataDriver::handleCleanUp()
{
    LOGP("handleCleanUp");
    if (mUtil)
    {
        PVFrameAndMetadataFactory::DeleteFrameAndMetadataUtility(mUtil);
        mUtil = NULL;
    }
#if BEST_THUMBNAIL_MODE
    delete mLocalDataSource;
    mLocalDataSource = NULL;
#endif
    delete mDataSource;
    mDataSource = NULL;

    OsclExecScheduler *sched=OsclExecScheduler::Current();
    if (sched) {
        sched->StopScheduler();
    }
}

// Retrieve all the available metadata keys.
void MetadataDriver::handleGetMetadataKeys()
{
    LOGP("handleGetMetadataKeys");
    int error = 0;
    mMetadataKeyList.clear();
    OSCL_TRY(error, mCmdId = mUtil->GetMetadataKeys(mMetadataKeyList, 0, -1, NULL, (OsclAny*)&mContextObject));
    OSCL_FIRST_CATCH_ANY(error, handleCommandFailure());
}

// Retrieve a frame and store the contents into an internal buffer.
void MetadataDriver::handleGetFrame()
{
    LOGP("handleGetFrame");
    int error = 0;
    mFrameBufferSize = MAX_VIDEO_FRAME_SIZE;
    OSCL_TRY(error, mCmdId = mUtil->GetFrame(mFrameSelector, mFrameBuffer, mFrameBufferSize, mFrameBufferProp, (OsclAny*)&mContextObject));
    OSCL_FIRST_CATCH_ANY(error, handleCommandFailure());
    LOGP("handleGetFrame out");
}

// Retrieve all the available metadata values associated with the given keys.
void MetadataDriver::handleGetMetadataValues()
{
    LOGP("handleGetMetadataValues");
    int error = 0;
    mNumMetadataValues = 0;
    mMetadataValueList.clear();
    trimKeys();  // Switch to use actual supported key list.
    OSCL_TRY(error, mCmdId = mUtil->GetMetadataValues(mActualMetadataKeyList, 0, -1, mNumMetadataValues, mMetadataValueList, (OsclAny*)&mContextObject));
    OSCL_FIRST_CATCH_ANY(error, handleCommandFailure());
}

void MetadataDriver::Run()
{
    LOGP("Run (%d)", mState);
    switch(mState) {
        case STATE_CREATE:
            handleCreate();
            break;
        case STATE_ADD_DATA_SOURCE:
            handleAddDataSource();
            break;
        case STATE_GET_METADATA_KEYS:
            handleGetMetadataKeys();
            break;
        case STATE_GET_METADATA_VALUES:
            handleGetMetadataValues();
            break;
        case STATE_GET_FRAME:
            handleGetFrame();
            break;
        case STATE_REMOVE_DATA_SOURCE:
            handleRemoveDataSource();
            break;
        default:
            handleCleanUp();
            break;
    }
}

bool MetadataDriver::isCommandSuccessful(const PVCmdResponse& aResponse) const
{
    LOGP("isCommandSuccessful");
    bool success = ((aResponse.GetCmdId() == mCmdId) &&
            (aResponse.GetCmdStatus() == PVMFSuccess) &&
            (aResponse.GetContext() == (OsclAny*)&mContextObject));
    if (!success) {
        LOGE("isCommandSuccessful: Command id(%d and expected %d) and status (%d and expected %d), data corruption (%s) at state (%d).",
             aResponse.GetCmdId(), mCmdId, aResponse.GetCmdStatus(), PVMFSuccess, (aResponse.GetContext() == (OsclAny*)&mContextObject)? "false": "true", mState);
    }
    return success;
}

void MetadataDriver::handleCommandFailure()
{
    LOGP("handleCommandFailure");
    if (mState == STATE_REMOVE_DATA_SOURCE) {
        mState = STATE_CLEANUP_AND_COMPLETE;
    }
    else{
        mState = STATE_REMOVE_DATA_SOURCE;
    }
    RunIfNotReady();
}

// Callback handler for a request completion by frameandmetadatautility.
void MetadataDriver::CommandCompleted(const PVCmdResponse& aResponse)
{
    LOGP("CommandCompleted (%d)", mState);
    if (!isCommandSuccessful(aResponse)) {
        handleCommandFailure();
        return;
    }

    switch(mState) {
        case STATE_ADD_DATA_SOURCE:
            if (mMode & GET_METADATA_ONLY) {
                mState = STATE_GET_METADATA_KEYS;
            } else if (mMode & GET_FRAME_ONLY) {
                mState = STATE_GET_FRAME;
            } else {
                LOGP("CommandCompleted: Neither retrieve metadata nor capture frame.");
                mState = STATE_REMOVE_DATA_SOURCE;
            }
            mIsSetDataSourceSuccessful = true;
            break;
        case STATE_GET_METADATA_KEYS:
            mState = STATE_GET_METADATA_VALUES;
            break;
        case STATE_GET_METADATA_VALUES:
            if (mMode & GET_FRAME_ONLY) {
                mState = STATE_GET_FRAME;
            } else {
                mState = STATE_REMOVE_DATA_SOURCE;
            }
            cacheMetadataRetrievalResults();
            break;
        case STATE_GET_FRAME:
            doColorConversion();
            mState = STATE_REMOVE_DATA_SOURCE;
            break;
        case STATE_REMOVE_DATA_SOURCE:
            mState = STATE_CLEANUP_AND_COMPLETE;
            break;
        default:
            mState = STATE_CLEANUP_AND_COMPLETE;
            break;
    }
    RunIfNotReady();
}

void MetadataDriver::HandleErrorEvent(const PVAsyncErrorEvent& aEvent)
{
    PVMFStatus status = aEvent.GetEventType();
    LOGE("HandleErrorEvent: %s", PVMFStatusToString(status));
    // Error occurs, clean up and terminate.
    LOGE("HandleErrorEvent: Event [type(%d), response type(%d)] received.", aEvent.GetEventType(), aEvent.GetResponseType());
    handleCommandFailure();
}


void MetadataDriver::HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent)
{
    LOGP("HandleInformationalEvent: Event [type(%d), response type(%d)] received.", aEvent.GetEventType(), aEvent.GetResponseType());
}


void MetadataDriver::closeSharedFdIfNecessary() {
    if (mSharedFd >= 0) {
        close(mSharedFd);
        mSharedFd = -1;
    }
}

//------------------------------------------------------------------------------
#include <media/PVMetadataRetriever.h>

namespace android {

//#define LOG_NDEBUG 0
#undef LOG_TAG
#define LOG_TAG "PVMetadataRetriever"

// A concrete subclass of MediaMetadataRetrieverInterface implementation
// Use the MetadataDriver object as a delegate and forward related calls
// to the MetadataDriver object.

extern "C" {
#include <pthread.h>
#include <semaphore.h>
}

static pthread_mutex_t METADATAMutex = PTHREAD_MUTEX_INITIALIZER;

PVMetadataRetriever::PVMetadataRetriever()
{
    LOG_PROFILE("***************************************");
    LOGD("========== PVMetadataRetriever constructor Start, tid = %d ==========\n", gettid());
    pthread_mutex_lock(&METADATAMutex);
    LOGD("========== PVMetadataRetriever constructor, tid = %d ==========\n", gettid());
    LOGP("constructor");
    mConstructorInTime = OsclTickCount::getTimeMs();
    LOG_PROFILE("constructor in, %lld", mConstructorInTime);
    mMetadataDriver = new MetadataDriver();
    LOG_PROFILE("constructor out, %lld", OsclTickCount::getTimeMs());
    LOGD("========== PVMetadataRetriever constructor End, tid = %d ==========\n", gettid());
}

PVMetadataRetriever::~PVMetadataRetriever()
{
    LOGP("destructor");
    LOGD("========== PVMetadataRetriever destructor Start, tid = %d ==========\n", gettid());
    Mutex::Autolock lock(mLock);
    delete mMetadataDriver;
    LOGD("========== PVMetadataRetriever destructor, tid = %d ==========\n", gettid());
    pthread_mutex_unlock(&METADATAMutex);
    LOGD("========== PVMetadataRetriever destructor End, tid = %d ==========\n", gettid());
}

status_t PVMetadataRetriever::setDataSource(const char *url)
{
    LOG_PROFILE("setDataSource %s", url);
    LOGP("setDataSource (%s)", url);

    Mutex::Autolock lock(mLock);
    if (mMetadataDriver == 0) {
        LOGE("No MetadataDriver available");
        return INVALID_OPERATION;
    }
    if (url == 0) {
        LOGE("Null pointer is passed as argument");
        return INVALID_OPERATION;
    }

    LOG_PROFILE("setDataSource in, %lld", OsclTickCount::getTimeMs());
    status_t s = mMetadataDriver->setDataSource(url);
    LOG_PROFILE("setDataSource out, %lld", OsclTickCount::getTimeMs());
    return s;
}

status_t PVMetadataRetriever::setDataSource(int fd, int64_t offset, int64_t length)
{
    LOGP("setDataSource fd(%d), offset(%lld), length(%lld)", fd, offset, length);

    Mutex::Autolock lock(mLock);
    if (mMetadataDriver == 0) {
        LOGE("No MetadataDriver available");
        return INVALID_OPERATION;
    }

    return mMetadataDriver->setDataSourceFd(fd, offset, length);
}

status_t PVMetadataRetriever::setMode(int mode)
{
    LOGP("setMode (%d)", mode);
    LOG_PROFILE("setMode in, %lld", OsclTickCount::getTimeMs());
    Mutex::Autolock lock(mLock);
    if (mMetadataDriver == 0) {
        LOGE("No MetadataDriver available");
        return NO_INIT;
    }
    if (mode < METADATA_MODE_NOOP ||
        mode > METADATA_MODE_FRAME_CAPTURE_AND_METADATA_RETRIEVAL) {
        LOGE("set to invalid mode (%d)", mode);
        return BAD_VALUE;
    }
    status_t s = mMetadataDriver->setMode(mode);
    LOG_PROFILE("setMode out, %lld", OsclTickCount::getTimeMs());
    return s;
}

status_t PVMetadataRetriever::getMode(int* mode) const
{
    LOGP("getMode");
    Mutex::Autolock lock(mLock);
    if (mMetadataDriver == 0) {
        LOGE("No MetadataDriver available");
        return INVALID_OPERATION;
    }
    if (mode == 0) {
        LOGE("Null pointer is passed as argument");
        return INVALID_OPERATION;
    }
    return mMetadataDriver->getMode(mode);
}

VideoFrame *PVMetadataRetriever::captureFrame()
{
    LOGP("captureFrame");
    Mutex::Autolock lock(mLock);
    if (mMetadataDriver == 0) {
        LOGE("No MetadataDriver available");
        return NULL;
    }

    LOG_PROFILE("captureFrame in, %lld", OsclTickCount::getTimeMs());
    VideoFrame* frame = mMetadataDriver->captureFrame();
    mCaptureFrameOutTime = OsclTickCount::getTimeMs();
    LOG_PROFILE("captureFrame out, %lld", mCaptureFrameOutTime);
    LOG_PROFILE("***************************************");
    LOG_PROFILE("capture one frame time: %d", mCaptureFrameOutTime - mConstructorInTime);
    return frame;
}

MediaAlbumArt *PVMetadataRetriever::extractAlbumArt()
{
    LOGP("extractAlbumArt");
    Mutex::Autolock lock(mLock);
    if (mMetadataDriver == 0) {
        LOGE("No MetadataDriver available");
        return NULL;
    }
    return mMetadataDriver->extractAlbumArt();
}

const char* PVMetadataRetriever::extractMetadata(int keyCode)
{
    LOGP("extractMetadata");
    Mutex::Autolock lock(mLock);
    if (mMetadataDriver == 0) {
        LOGE("No MetadataDriver available");
        return NULL;
    }
    return mMetadataDriver->extractMetadata(keyCode);
}

};  // android
