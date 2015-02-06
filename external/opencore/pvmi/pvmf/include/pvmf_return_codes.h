


#ifndef PVMF_RETURN_CODES_H_INCLUDED
#define PVMF_RETURN_CODES_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif


typedef int32 PVMFStatus;

// Return codes
const PVMFStatus PVMFSuccess = 1;
const PVMFStatus PVMFPending = 0;
const PVMFStatus PVMFNotSet = 2;


// Error codes (negative values)

const PVMFStatus PVMFErrFirst = (-1);
const PVMFStatus PVMFFailure = (-1);
const PVMFStatus PVMFErrCancelled = (-2);
const PVMFStatus PVMFErrNoMemory = (-3);
const PVMFStatus PVMFErrNotSupported = (-4);
const PVMFStatus PVMFErrArgument = (-5);
const PVMFStatus PVMFErrBadHandle = (-6);
const PVMFStatus PVMFErrAlreadyExists = (-7);
const PVMFStatus PVMFErrBusy = (-8);
const PVMFStatus PVMFErrNotReady = (-9);
const PVMFStatus PVMFErrCorrupt = (-10);
const PVMFStatus PVMFErrTimeout = (-11);
const PVMFStatus PVMFErrOverflow = (-12);
const PVMFStatus PVMFErrUnderflow = (-13);
const PVMFStatus PVMFErrInvalidState = (-14);
const PVMFStatus PVMFErrNoResources = (-15);
const PVMFStatus PVMFErrResourceConfiguration = (-16);
const PVMFStatus PVMFErrResource = (-17);
const PVMFStatus PVMFErrProcessing = (-18);
const PVMFStatus PVMFErrPortProcessing = (-19);
const PVMFStatus PVMFErrAccessDenied = (-20);
const PVMFStatus PVMFErrLicenseRequired = (-21);
const PVMFStatus PVMFErrLicenseRequiredPreviewAvailable = (-22);
const PVMFStatus PVMFErrContentTooLarge = (-23);
const PVMFStatus PVMFErrMaxReached = (-24);
const PVMFStatus PVMFLowDiskSpace = (-25);
const PVMFStatus PVMFErrHTTPAuthenticationRequired = (-26);
const PVMFStatus PVMFErrCallbackHasBecomeInvalid = (-27);
const PVMFStatus PVMFErrCallbackClockStopped = (-28);
const PVMFStatus PVMFErrReleaseMetadataValueNotDone = (-29);
const PVMFStatus PVMFErrRedirect = (-30);
const PVMFStatus PVMFErrNotImplemented = (-31);
const PVMFStatus PVMFErrContentInvalidForProgressivePlayback = (-32);
// <--- Demon Deng
const PVMFStatus PVMFErrNetwork = (-99);
// --->
const PVMFStatus PVMFErrLast = (-100);
#define IsPVMFErrCode(s) ((PVMFErrLast<=s)&&(s<=PVMFErrFirst))

// Informational codes (positive values)

const PVMFStatus PVMFInfoFirst = 10;

const PVMFStatus PVMFInfoPortCreated = 10;
const PVMFStatus PVMFInfoPortDeleted = 11;
const PVMFStatus PVMFInfoPortConnected = 12;
const PVMFStatus PVMFInfoPortDisconnected = 13;
const PVMFStatus PVMFInfoOverflow = 14;
const PVMFStatus PVMFInfoUnderflow = 15;
const PVMFStatus PVMFInfoProcessingFailure = 16;
const PVMFStatus PVMFInfoEndOfData = 17;
const PVMFStatus PVMFInfoBufferCreated = 18;
const PVMFStatus PVMFInfoBufferingStart = 19;
const PVMFStatus PVMFInfoBufferingStatus = 20;
const PVMFStatus PVMFInfoBufferingComplete = 21;
const PVMFStatus PVMFInfoDataReady = 22;
const PVMFStatus PVMFInfoPositionStatus = 23;
const PVMFStatus PVMFInfoStateChanged = 24;
const PVMFStatus PVMFInfoDataDiscarded = 25;
const PVMFStatus PVMFInfoErrorHandlingStart = 26;
const PVMFStatus PVMFInfoErrorHandlingComplete = 27;
const PVMFStatus PVMFInfoRemoteSourceNotification = 28;
const PVMFStatus PVMFInfoLicenseAcquisitionStarted = 29;
const PVMFStatus PVMFInfoContentLength = 30;
const PVMFStatus PVMFInfoContentTruncated = 31;
const PVMFStatus PVMFInfoSourceFormatNotSupported = 32;
const PVMFStatus PVMFInfoPlayListClipTransition = 33;
const PVMFStatus PVMFInfoContentType = 34;
const PVMFStatus PVMFInfoTrackDisable = 35;
const PVMFStatus PVMFInfoUnexpectedData = 36;
const PVMFStatus PVMFInfoSessionDisconnect = 37;
const PVMFStatus PVMFInfoStartOfData = 38;
const PVMFStatus PVMFInfoReportObserverRecieved = 39;
const PVMFStatus PVMFInfoMetadataAvailable = 40;
const PVMFStatus PVMFInfoDurationAvailable = 41;
const PVMFStatus PVMFInfoChangePlaybackPositionNotSupported = 42;
const PVMFStatus PVMFInfoPoorlyInterleavedContent = 43;
const PVMFStatus PVMFInfoActualPlaybackPosition = 44;
const PVMFStatus PVMFInfoLiveBufferEmpty = 45;
const PVMFStatus PVMFInfoPlayListSwitch = 46;
const PVMFStatus PVMFMIOConfigurationComplete = 47;
const PVMFStatus PVMFInfoVideoTrackFallingBehind = 48;
const PVMFStatus PVMFInfoSourceOverflow = 49;
const PVMFStatus PVMFInfoShoutcastMediaDataLength = 50;
const PVMFStatus PVMFInfoShoutcastClipBitrate = 51;
const PVMFStatus PVMFInfoIsShoutcastSesssion = 52;
const PVMFStatus PVMFInfoBufNotReturn = 53;

// <--- Morris Yang ALPS00006721 [
const PVMFStatus PVMFInfoNotSeekable = 54;
// --->

// <--- Morris Yang ALPS00135233
const PVMFStatus PVMFInfoDownloadingMoov = 55;
// --->

// <--- sam sun for aac seek table gen [
const PVMFStatus PVMFInfoSeekable = 56;
// --->

// <--- Demon Deng ALPS00136342
const PVMFStatus PVMFInfoDownloadedMoov = 98;
// --->

const PVMFStatus PVMFInfoSinkThreadCreate = 99;

const PVMFStatus PVMFInfoLast = 100;
#define IsPVMFInfoCode(s) ((PVMFInfoFirst<=s)&&(s<=PVMFInfoLast))

// Convert a PVMFStatus code to a string that can be used in logs.
// @param status code.
// @return a human readable string representing the status.
OSCL_IMPORT_REF const char *PVMFStatusToString(const PVMFStatus status);

#endif
