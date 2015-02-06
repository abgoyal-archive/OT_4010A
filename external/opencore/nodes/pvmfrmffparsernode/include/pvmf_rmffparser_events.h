 
 /*****************************************************************************
 *
 * Filename:
 * ---------
 *   pvmf_rmffparser_events.h
 *
 * Project:
 * --------
 *   YUSU
 *
 * Description:
 * ------------
 *   Implement RMFF parser node
 *
 * Author:
 * -------
 *   Morris Yang (mtk03147)
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision$
 * $Modtime $
 * $Log     $  
 *
 * 01 13 2011 donglei.ji
 * [ALPS00139474] [Need Patch] [Volunteer Patch] Android2.3 opencore migration
 * Android2.3 opencore migration.
 *
 * 08 13 2010 techien.chen
 * [ALPS00003391] [Need Patch] [Volunteer Patch] ALPS.W33 Volunteer patch for Android 2.2 migration, opencore part.
 * opencore 2.2 migration. Disable video HW related in this version.
 *
 * 08 13 2010 techien.chen
 * [ALPS00003391] [Need Patch] [Volunteer Patch] ALPS.W33 Volunteer patch for Android 2.2 migration, opencore part.
 * opencore 2.2 migration. Disable video HW related in this version.
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#ifndef PVMF_RMFFPARSER_EVENTS_H_INCLUDED
#define PVMF_RMFFPARSER_EVENTS_H_INCLUDED

#define PVMFEventTypesUUID PVUuid(0xf86cd94a,0x3b18,0x4597,0x9f,0x54,0xb8,0x0b,0x29,0x4c,0xbb,0x12)

typedef enum
{
    /**
     When MP4 FF reports error READ_USER_DATA_ATOM_FAILED
    **/
    PVMFRMFFParserErrUserDataAtomReadFailed = 1024,

    /**
     When MP4 FF reports error READ_MEDIA_DATA_ATOM_FAILED
    **/
    PVMFRMFFParserErrMediaDataAtomReadFailed,

    /**
     When MP4 FF reports error READ_MOVIE_ATOM_FAILED
    **/
    PVMFRMFFParserErrMovieAtomReadFailed,

    /**
     When MP4 FF reports error READ_MOVIE_HEADER_ATOM_FAILED
    **/
    PVMFRMFFParserErrMovieHeaderAtomReadFailed,

    /**
     When MP4 FF reports error READ_TRACK_ATOM_FAILED
    **/
    PVMFRMFFParserErrTrackAtomReadFailed,

    /**
     When MP4 FF reports error READ_TRACK_HEADER_ATOM_FAILED
    **/
    PVMFRMFFParserErrTrackHeaderAtomReadFailed,

    /**
     When MP4 FF reports error READ_TRACK_REFERENCE_ATOM_FAILED
    **/
    PVMFRMFFParserErrTrackReferenceAtomReadFailed,

    /**
     When MP4 FF reports error READ_TRACK_REFERENCE_TYPE_ATOM_FAILED
    **/
    PVMFRMFFParserErrTrackReferenceTypeAtomReadFailed,

    /**
     When MP4 FF reports error READ_OBJECT_DESCRIPTOR_ATOM_FAILED
    **/
    PVMFRMFFParserErrObjectDescriptorAtomReadFailed,

    /**
     When MP4 FF reports error READ_INITIAL_OBJECT_DESCRIPTOR_FAILED
    **/
    PVMFRMFFParserErrInitialObjectDescriptorReadFailed,

    /**
     When MP4 FF reports error READ_OBJECT_DESCRIPTOR_FAILED
    **/
    PVMFRMFFParserErrObjectDescriptorReadFailed,

    /**
     When MP4 FF reports error READ_MEDIA_ATOM_FAILED
    **/
    PVMFRMFFParserErrMediaAtomReadFailed,

    /**
     When MP4 FF reports error READ_MEDIA_HEADER_ATOM_FAILED
    **/
    PVMFRMFFParserErrMediaHeaderAtomReadFailed,

    /**
     When MP4 FF reports error READ_HANDLER_ATOM_FAILED
    **/
    PVMFRMFFParserErrHandlerAtomReadFailed,

    /**
     When MP4 FF reports error READ_MEDIA_INFORMATION_ATOM_FAILED
    **/
    PVMFRMFFParserErrMediaInformationAtomReadFailed,

    /**
     When MP4 FF reports error READ_MEDIA_INFORMATION_HEADER_ATOM_FAILED
    **/
    PVMFRMFFParserErrMediaInformationHeaderAtomReadFailed,

    /**
     When MP4 FF reports error READ_VIDEO_MEDIA_HEADER_ATOM_FAILED
    **/
    PVMFRMFFParserErrVideoMediaHeaderAtomReadFailed,

    /**
     When MP4 FF reports error READ_SOUND_MEDIA_HEADER_ATOM_FAILED
    **/
    PVMFRMFFParserErrSoundMediaHeaderAtomReadFailed,

    /**
     When MP4 FF reports error READ_HINT_MEDIA_HEADER_ATOM_FAILED
    **/
    PVMFRMFFParserErrHintMediaHeaderAtomReadFailed,

    /**
     When MP4 FF reports error READ_MPEG4_MEDIA_HEADER_ATOM_FAILED
    **/
    PVMFRMFFParserErrMPEG4MediaHeaderAtomReadFailed,

    /**
     When MP4 FF reports error READ_DATA_INFORMATION_ATOM_FAILED
    **/
    PVMFRMFFParserErrDataInformationAtomReadFailed,

    /**
     When MP4 FF reports error READ_DATA_REFERENCE_ATOM_FAILED
    **/
    PVMFRMFFParserErrDataReferenceAtomReadFailed,

    /**
     When MP4 FF reports error READ_DATA_ENTRY_URL_ATOM_FAILED
    **/
    PVMFRMFFParserErrDataEntryURLAtomReadFailed,

    /**
     When MP4 FF reports error READ_DATA_ENTRY_URN_ATOM_FAILED
    **/
    PVMFRMFFParserErrDataEntryURNAtomReadFailed,

    /**
     When MP4 FF reports error READ_SAMPLE_TABLE_ATOM_FAILED
    **/
    PVMFRMFFParserErrSampleTableAtomReadFailed,

    /**
     When MP4 FF reports error READ_TIME_TO_SAMPLE_ATOM_FAILED
    **/
    PVMFRMFFParserErrTimeToSampleAtomReadFailed,

    /**
     When MP4 FF reports error READ_SAMPLE_DESCRIPTION_ATOM_FAILED
    **/
    PVMFRMFFParserErrSampleDescriptionAtomReadFailed,

    /**
     When MP4 FF reports error READ_SAMPLE_SIZE_ATOM_FAILED
    **/
    PVMFRMFFParserErrSampleSizeAtomReadFailed,

    /**
     When MP4 FF reports error READ_SAMPLE_TO_CHUNK_ATOM_FAILED
    **/
    PVMFRMFFParserErrSampleToChunkAtomReadFailed,

    /**
     When MP4 FF reports error READ_CHUNK_OFFSET_ATOM_FAILED
    **/
    PVMFRMFFParserErrChunkOffsetAtomReadFailed,

    /**
     When MP4 FF reports error READ_SYNC_SAMPLE_ATOM_FAILED
    **/
    PVMFRMFFParserErrSyncSampleAtomReadFailed,

    /**
     When MP4 FF reports error READ_SAMPLE_ENTRY_FAILED
    **/
    PVMFRMFFParserErrSampleEntryReadFailed,

    /**
     When MP4 FF reports error READ_AUDIO_SAMPLE_ENTRY_FAILED
    **/
    PVMFRMFFParserErrAudioSampleEntryReadFailed,

    /**
     When MP4 FF reports error READ_VISUAL_SAMPLE_ENTRY_FAILED
    **/
    PVMFRMFFParserErrVisualSampleEntryReadFailed,

    /**
     When MP4 FF reports error READ_HINT_SAMPLE_ENTRY_FAILED
    **/
    PVMFRMFFParserErrHintSampleEntryReadFailed,

    /**
     When MP4 FF reports error READ_MPEG_SAMPLE_ENTRY_FAILED
    **/
    PVMFRMFFParserErrMPEGSampleEntryReadFailed,

    /**
     When MP4 FF reports error READ_AUDIO_HINT_SAMPLE_FAILED
    **/
    PVMFRMFFParserErrAudioHintSampleReadFailed,

    /**
     When MP4 FF reports error READ_VIDEO_HINT_SAMPLE_FAILED
    **/
    PVMFRMFFParserErrVideoHintSampleReadFailed,

    /**
     When MP4 FF reports error READ_ESD_ATOM_FAILED
    **/
    PVMFRMFFParserErrESDAtomReadFailed,

    /**
     When MP4 FF reports error READ_ES_DESCRIPTOR_FAILED
    **/
    PVMFRMFFParserErrESDescriptorReadFailed,

    /**
     When MP4 FF reports error READ_SL_CONFIG_DESCRIPTOR_FAILED
    **/
    PVMFRMFFParserErrSLConfigDescriptorReadFailed,

    /**
     When MP4 FF reports error READ_DECODER_CONFIG_DESCRIPTOR_FAILED
    **/
    PVMFRMFFParserErrDecoderConfigDescriptorReadFailed,

    /**
     When MP4 FF reports error READ_DECODER_SPECIFIC_INFO_FAILED
    **/
    PVMFRMFFParserErrDecoderSpecificInfoReadFailed,

    /**
     When MP4 FF reports error READ_FILE_TYPE_ATOM_FAILED
    **/
    PVMFRMFFParserErrFileTypeAtomReadFailed,

    /**
     When MP4 FF reports error READ_PVTI_SESSION_INFO_FAILED
    **/
    PVMFRMFFParserErrPVTISessionInfoReadFailed,

    /**
     When MP4 FF reports error READ_PVTI_MEDIA_INFO_FAILED
    **/
    PVMFRMFFParserErrPVTIMediaInfoReadFailed,

    /**
     When MP4 FF reports error READ_CONTENT_VERSION_FAILED
    **/
    PVMFRMFFParserErrContentVersionReadFailed,

    /**
     When MP4 FF reports error READ_DOWNLOAD_ATOM_FAILED
    **/
    PVMFRMFFParserErrDownloadAtomReadFailed,

    /**
     When MP4 FF reports error READ_TRACK_INFO_ATOM_FAILED
    **/
    PVMFRMFFParserErrTrackInfoAtomReadFailed,

    /**
     When MP4 FF reports error READ_REQUIREMENTS_ATOM_FAILED
    **/
    PVMFRMFFParserErrRequirementsAtomReadFailed,

    /**
     When MP4 FF reports error READ_WMF_SET_MEDIA_ATOM_FAILED
    **/
    PVMFRMFFParserErrWMFSetMediaAtomReadFailed,

    /**
     When MP4 FF reports error READ_WMF_SET_SESSION_ATOM_FAILED
    **/
    PVMFRMFFParserErrWMFSetSessionAtomReadFailed,

    /**
     When MP4 FF reports error READ_PV_CONTENT_TYPE_ATOM_FAILED
    **/
    PVMFRMFFParserErrPVContentTypeAtomReadFailed,

    /**
     When MP4 FF reports error READ_PV_USER_DATA_ATOM_FAILED
    **/
    PVMFRMFFParserErrPVUserDataAtomReadFailed,

    /**
     When MP4 FF reports error READ_VIDEO_INFORMATION_ATOM_FAILED
    **/
    PVMFRMFFParserErrVideoInformationAtomReadFailed,

    /**
     When MP4 FF reports error READ_RANDOM_ACCESS_ATOM_FAILED
    **/
    PVMFRMFFParserErrRandomAccessAtomReadFailed,

    /**
     When MP4 FF reports error READ_AMR_SAMPLE_ENTRY_FAILED
    **/
    PVMFRMFFParserErrAMRSampleEntryReadFailed,

    /**
     When MP4 FF reports error READ_H263_SAMPLE_ENTRY_FAILED
    **/
    PVMFRMFFParserErrH263SampleEntryReadFailed,

    /**
     When MP4 FF reports error READ_UUID_ATOM_FAILED
    **/
    PVMFRMFFParserErrUUIDAtomReadFailed,

    /**
     When MP4 FF reports error READ_DRM_ATOM_FAILED
    **/
    PVMFRMFFParserErrDRMAtomReadFailed,

    /**
     When MP4 FF reports error READ_DCMD_ATOM_FAILED
    **/
    PVMFRMFFParserErrDCMDAtomReadFailed,

    /**
     When MP4 FF reports error READ_COPYRIGHT_ATOM_FAILED
    **/
    PVMFRMFFParserErrCopyrightAtomReadFailed,

    /**
     When MP4 FF reports error READ_FONT_TABLE_ATOM_FAILED
    **/
    PVMFRMFFParserErrFontTableAtomReadFailed,

    /**
     When MP4 FF reports error READ_FONT_RECORD_FAILED
    **/
    PVMFRMFFParserErrFontRecordReadFailed,

    /**
     When MP4 FF reports error READ_PV_ENTITY_TAG_ATOM_FAILED
    **/
    PVMFRMFFParserErrPVEntityTagAtomReadFailed,

    /**
     When MP4 FF reports error READ_EDIT_ATOM_FAILED
    **/
    PVMFRMFFParserErrEditAtomReadFailed,

    /**
     When MP4 FF reports error READ_EDITLIST_ATOM_FAILED
    **/
    PVMFRMFFParserErrEditlistAtomReadFailed,

    /**
     When MP4 FF reports error READ_UDTA_TITL_FAILED
    **/
    PVMFRMFFParserErrUDTATITLReadFailed,

    /**
     When MP4 FF reports error READ_UDTA_DSCP_FAILED
    **/
    PVMFRMFFParserErrUDTADSCPReadFailed,

    /**
     When MP4 FF reports error READ_UDTA_CPRT_FAILED
    **/
    PVMFRMFFParserErrUDTACPRTReadFailed,

    /**
     When MP4 FF reports error READ_UDTA_PERF_FAILED
    **/
    PVMFRMFFParserErrUDTAPERFReadFailed,

    /**
     When MP4 FF reports error READ_UDTA_AUTH_FAILED
    **/
    PVMFRMFFParserErrUDTAUTHReadFailed,

    /**
     When MP4 FF reports error READ_UDTA_GNRE_FAILED
    **/
    PVMFRMFFParserErrUDTAGNREReadFailed,

    /**
     When MP4 FF reports error READ_UDTA_RTNG_FAILED
    **/
    PVMFRMFFParserErrUDTARTNGReadFailed,

    /**
     When MP4 FF reports error READ_UDTA_CLSF_FAILED
    **/
    PVMFRMFFParserErrUDTACLSFReadFailed,

    /**
     When MP4 FF reports error READ_UDTA_KYWD_FAILED
    **/
    PVMFRMFFParserErrUDTAKYWDReadFailed,

    /**
     When MP4 FF reports error DUPLICATE_MOVIE_ATOMS
    **/
    PVMFRMFFParserErrDuplicateMovieAtoms,

    /**
     When MP4 FF reports error NO_MOVIE_ATOM_PRESENT
    **/
    PVMFRMFFParserErrNoMovieAtomPresent,

    /**
     When MP4 FF reports error DUPLICATE_OBJECT_DESCRIPTORS
    **/
    PVMFRMFFParserErrDuplicateObjectDescriptors,

    /**
     When MP4 FF reports error NO_OBJECT_DESCRIPTOR_ATOM_PRESENT
    **/
    PVMFRMFFParserErrNoObjectDescriptorAtomPresent,

    /**
     When MP4 FF reports error DUPLICATE_MOVIE_HEADERS
    **/
    PVMFRMFFParserErrDuplicateMovieHeaders,

    /**
     When MP4 FF reports error NO_MOVIE_HEADER_ATOM_PRESENT
    **/
    PVMFRMFFParserErrNoMovieHeaderAtomPresent,

    /**
     When MP4 FF reports error DUPLICATE_TRACK_REFERENCE_ATOMS
    **/
    PVMFRMFFParserErrDuplicateTrackReferenceAtoms,

    /**
     When MP4 FF reports error DUPLICATE_TRACK_HEADER_ATOMS
    **/
    PVMFRMFFParserErrDuplicateTrackHeaderAtoms,

    /**
     When MP4 FF reports error NO_TRACK_HEADER_ATOM_PRESENT
    **/
    PVMFRMFFParserErrNoTrackHeaderAtomPresent,

    /**
     When MP4 FF reports error DUPLICATE_MEDIA_ATOMS
    **/
    PVMFRMFFParserErrDuplicateMediaAtoms,

    /**
     When MP4 FF reports error NO_MEDIA_ATOM_PRESENT
    **/
    PVMFRMFFParserErrNoMediaAtomPresent,

    /**
     When MP4 FF reports error READ_UNKOWN_ATOM
    **/
    PVMFRMFFParserErrUnknownAtom,

    /**
     When MP4 FF reports error NON_PV_CONTENT
    **/
    PVMFRMFFParserErrNonPVContent,

    /**
     When MP4 FF reports error FILE_NOT_STREAMABLE
    **/
    PVMFRMFFParserErrFileNotStreamable,

    /**
     When MP4 FF reports error INVALID_SAMPLE_SIZE
    **/
    PVMFRMFFParserErrInvalidSampleSize,

    /**
     When MP4 FF reports error INVALID_CHUNK_OFFSET
    **/
    PVMFRMFFParserErrInvalidChunkOffset,

    /**
     When MP4 FF reports error MEMORY_ALLOCATION_FAILED
    **/
    PVMFRMFFParserErrMemoryAllocationFailed,

    /**
     When MP4 FF reports error ZERO_OR_NEGATIVE_ATOM_SIZE
    **/
    PVMFRMFFParserErrZeroOrNegativeAtomSize,

    /**
     When MP4 FF reports error NO_MEDIA_TRACKS_IN_FILE
    **/
    PVMFRMFFParserErrNoMediaTracksInFile,

    /**
     When MP4 FF reports error NO_META_DATA_FOR_MEDIA_TRACKS
    **/
    PVMFRMFFParserErrNoMetadataForMediaTracks,

    /**
     When MP4 FF reports error MEDIA_DATA_NOT_SELF_CONTAINED
    **/
    PVMFRMFFParserErrMediaDataNotSelfContained,

    /**
     When MP4 FF reports error FILE_VERSION_NOT_SUPPORTED
    **/
    PVMFRMFFParserErrFileVersionNotSupported,

    /**
     When MP4 FF reports error TRACK_VERSION_NOT_SUPPORTED
    **/
    PVMFRMFFParserErrTrackVersioNotSupported,

    /**
     When MP4 FF reports error FILE_PSEUDO_STREAMABLE
    **/
    PVMFRMFFParserErrPseudostreamableFile,

    /**
     When MP4 FF reports error FILE_NOT_PSEUDO_STREAMABLE
    **/
    PVMFRMFFParserErrNotPseudostreamableFile,

    /**
     When MP4 FF reports error DUPLICATE_FILE_TYPE_ATOMS
    **/
    PVMFRMFFParserErrDuplicateFileTypeAtoms,

    /**
     When MP4 FF reports error UNSUPPORTED_FILE_TYPE
    **/
    PVMFRMFFParserErrUnsupportedFileType,

    /**
     When MP4 FF reports error FILE_TYPE_ATOM_NOT_FOUND
    **/
    PVMFRMFFParserErrFileTypeAtomNotFound,

    /**
     When MP4 FF reports error ATOM_VERSION_NOT_SUPPORTED
    **/
    PVMFRMFFParserErrAtomVersionNotSupported,

    /**
     When MP4 FF reports error READ_AVC_SAMPLE_ENTRY_FAILED
    **/
    PVMFRMFFParserErrReadAVCSampleEntryFailed,

    /**
     When MP4 FF reports error READ_AVC_CONFIG_BOX_FAILED
    **/
    PVMFRMFFParserErrReadAVCConfigBoxFailed,

    /**
     When MP4 FF reports error READ_MPEG4_BITRATE_BOX_FAILED
    **/
    PVMFRMFFParserErrReadMPEG4BitRateBoxFailed,

    /**
     Placeholder for the last PV MP4 FF parser error event
     **/
    PVMFRMFFParserErrLast = 8191
} PVMFRMFFParserErrorEventType;

typedef enum
{
    /**
     MP4 file is pseudostreamable
     **/
    PVMFRMFFParserInfoPseudostreamableFile = 8192,

    /**
     MP4 file is non-pseudostreamable
     **/
    PVMFRMFFParserInfoNotPseudostreamableFile,

    /**
     Placeholder for the last PV MP4 FF parser informational event
     **/
    PVMFRMFFParserInfoLast = 10000

} PVMFRMFFParserInformationalEventType;

#endif // PVMF_RMFFPARSER_EVENTS_H_INCLUDED


