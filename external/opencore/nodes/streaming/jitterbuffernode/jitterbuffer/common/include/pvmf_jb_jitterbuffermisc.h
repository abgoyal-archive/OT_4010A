
#ifndef PVMF_JB_JITTERBUFFERMISC_H_INCLUDED
#define PVMF_JB_JITTERBUFFERMISC_H_INCLUDED

#ifndef PVMF_MEDIA_CLOCK_H_INCLUDED
#include "pvmf_media_clock.h"
#endif

#ifndef PVMF_JB_EVENT_NOTIFIER_H
#include "pvmf_jb_event_notifier.h"
#endif

#ifndef PVMF_JB_SESSION_DURATION_TIMER_H_INCLUDED
#include "pvmf_jb_session_duration_timer.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef PVMF_RTCP_PROTO_IMPL_H_INCLUDED
#include "pvmf_rtcp_proto_impl.h"
#endif

#ifndef PVMFFIREWALLPKTS_H_INCLUDED
#include "pvmf_jb_firewall_pkts_impl.h"
#endif

#ifndef PVMF_JITTER_BUFFER_PORT_H_INCLUDED
#include "pvmf_jitter_buffer_port.h"
#endif

#ifndef RTSP_TIME_FORMAT_H
#include "rtsp_time_formats.h"
#endif


#include <cutils/log.h>


class PVLogger;

class PVMFJitterBufferMiscObserver
{
    public:
        virtual void MessageReadyToSend(PVMFPortInterface*& aPort, PVMFSharedMediaMsgPtr& aMessage) = 0;
        virtual void MediaReceivingChannelPrepared(bool aStatus) = 0;
        virtual void ProcessRTCPControllerEvent(PVMFAsyncEvent& aEvent) = 0;
        virtual void SessionSessionExpired() = 0;
};

class PVMFJitterBufferMisc: public PVRTCPProtoImplementorObserver
        , public PVMFJBEventNotifierObserver
        , public PvmfJBSessionDurationTimerObserver
{
    public:
        OSCL_IMPORT_REF static PVMFJitterBufferMisc* New(PVMFJitterBufferMiscObserver* aObserver, PVMFMediaClock& aClientPlaybackClock, Oscl_Vector<PVMFJitterBufferPortParams*, OsclMemAllocator>& aPortParamsQueue);
        OSCL_IMPORT_REF virtual ~PVMFJitterBufferMisc();
        OSCL_IMPORT_REF void StreamingSessionStarted();
        OSCL_IMPORT_REF void StreamingSessionStopped();
        OSCL_IMPORT_REF void StreamingSessionPaused();
        OSCL_IMPORT_REF void StreamingSessionBufferingStart();
        OSCL_IMPORT_REF void StreamingSessionBufferingEnd();

        OSCL_IMPORT_REF void SetBroadcastSession();

        OSCL_IMPORT_REF void ResetEstimatedServerClock();

        OSCL_IMPORT_REF bool PrepareForRepositioning(bool oUseExpectedClientClockVal, uint32 aExpectedClientClockVal);
        OSCL_IMPORT_REF bool PurgeElementsWithNPTLessThan(NptTimeFormat &aNPTTime);

        OSCL_IMPORT_REF bool IsSessionExpired();

        OSCL_IMPORT_REF void Prepare();
        OSCL_IMPORT_REF void Reset();

        OSCL_IMPORT_REF PVMFStatus PrepareMediaReceivingChannel();
        OSCL_IMPORT_REF PVMFStatus CancelMediaReceivingChannelPreparation();

        OSCL_IMPORT_REF PVMFStatus ProcessFeedbackMessage(PVMFJitterBufferPortParams& aParam, PVMFSharedMediaMsgPtr aMsg);
        OSCL_IMPORT_REF void SetRateAdaptationInfo(PVMFPortInterface* aPort, bool aRateAdaptation, uint32 aRateAdaptationFeedBackFrequency, uint32 aRateAdaptationFreeBufferSpaceInBytes = 0);
        OSCL_IMPORT_REF void SetRTCPIntervalInMicroSecs(uint32 aRTCPInterval);
        OSCL_IMPORT_REF bool SetPortRTCPParams(PVMFPortInterface* aPort, int aNumSenders, uint32 aRR, uint32 aRS);
        OSCL_IMPORT_REF bool ResetSession();
        OSCL_IMPORT_REF bool SetSessionDurationExpired();

        OSCL_IMPORT_REF void MediaReceivingChannelPreparationRequired(bool aRequired);
        OSCL_IMPORT_REF PVMFMediaClock& GetEstimatedServerClock();
        OSCL_IMPORT_REF PVMFJBEventNotifier* GetEventNotifier();

        OSCL_IMPORT_REF bool SetPlayRange(int32 aStartTimeInMS,
                                          int32 aStopTimeInMS,
                                          bool aPlayAfterASeek,
                                          bool aStopTimeAvailable);

        OSCL_IMPORT_REF void SetPortSSRC(PVMFPortInterface* aPort, uint32 aSSRC);

        OSCL_IMPORT_REF uint32 GetEstimatedServerClockValue();

        OSCL_IMPORT_REF bool PlayStopTimeAvailable() const;

        OSCL_IMPORT_REF void SetServerInfo(PVMFJitterBufferFireWallPacketInfo& aServerInfo);

        OSCL_IMPORT_REF PVMFTimestamp GetMaxMediaDataTS();
        OSCL_IMPORT_REF PVMFTimestamp GetActualMediaDataTSAfterSeek();

        OSCL_IMPORT_REF void SetMediaClockConverter(PVMFPortInterface* apPort, MediaClockConverter* apMediaClockConverter);
        OSCL_IMPORT_REF void ProcessFirstPacketAfterSeek();

        void ProcessCallback(CLOCK_NOTIFICATION_INTF_TYPE aClockNotificationInterfaceType, uint32 aCallBkId, const OsclAny* aContext, PVMFStatus aStatus);
        virtual void PVMFJBSessionDurationTimerEvent();
        PVMFStatus RTCPPacketReceived(RTCPPacketType aPacketType, PVRTCPChannelController* aController);
        PVMFStatus RTCPReportReadyToSend(PVMFPortInterface*& aPort, PVMFSharedMediaMsgPtr& aMessage);
        PVMFStatus ProcessInfoEvent(PVMFAsyncEvent& aEvent);
        void LogClientAndEstimatedServerClock(PVLogger* aLogger);

	uint32 GetSessionDuration()	{	return iSessionDuration;	}
	void SetSessionDuration(uint32 duration)	{	iSessionDuration = duration;	}

	void DisableRTCPFeedback() { iDisableRTCPFeedback = true; }


	// <--- Morris Yang ALPS00225213, ALPS00225214
	PVFirewallPacketExchangeImpl* GetFireWallPacketExchangerImpl() { return ipFireWallPacketExchangerImpl; }
	// --->


    private:
        PVMFJitterBufferMisc(PVMFJitterBufferMiscObserver* aObserver, PVMFMediaClock& aClientPlaybackClock, Oscl_Vector<PVMFJitterBufferPortParams*, OsclMemAllocator>& aPortParamsQueue):
                irClientPlaybackClock(aClientPlaybackClock)
                , ipObserver(aObserver)
                , irPortParamsQueue(aPortParamsQueue)
                , ipSessionDurationTimer(NULL)
                , ipEventNotifier(NULL)
                , ipRTCPProtoImplementator(NULL)
                , ipFireWallPacketExchangerImpl(NULL)
                , ipEstimatedServerClock(NULL)
                , ipWallClock(NULL)
                , ipNonDecreasingClock(NULL)
                , ipJBEventsClockLogger(NULL)
                , ipClockLoggerSessionDuration(NULL)
                , ipClockLogger(NULL)
                , ipDataPathLoggerIn(NULL)
                , ipDataPathLoggerOut(NULL)
                , ipDataPathLoggerRTCP(NULL)
                , ipLogger(NULL)
                , iDisableRTCPFeedback(false)
        {
            ResetParams(false);
    // <--- Morris Yang ALPS00225250
            iLastBufferingStartTime = 0;
            iLastBufferingTime = 0;
    // --->
        }

        void Construct();
        bool CreateProtocolObjects();
        bool LookupRTCPChannelParams(PVMFPortInterface* rtpPort, PVMFPortInterface*& rtcpPort, PVMFJitterBuffer*& rtpPktJitterBuffer);
        bool LocateFeedBackPort(PVMFJitterBufferPortParams*& aInputPortParamsPtr, PVMFJitterBufferPortParams*& aFeedBackPortParamsPtr);
        bool FirewallPacketExchangerRequired() const;
        bool RTCPProtocolImplementorRequired() const;

        void ResetParams(bool aReleaseMemory = true);
        void ComputeCurrentSessionDurationMonitoringInterval()
        {
            /* Restart the session duration timer after accounting for any elapsed time */
            uint64 elapsedTime = ipSessionDurationTimer->GetMonitoringIntervalElapsed();
            uint32 elapsedTime32 = Oscl_Int64_Utils::get_uint64_lower32(elapsedTime);
            ipSessionDurationTimer->UpdateElapsedSessionDuration(elapsedTime32);
            uint32 totalSessionDuration = ipSessionDurationTimer->getSessionDurationInMS();
            uint32 elapsedSessionDuration = ipSessionDurationTimer->GetElapsedSessionDurationInMS();
            uint32 interval = (totalSessionDuration - elapsedSessionDuration);
            if (interval > PVMF_JITTER_BUFFER_NODE_SESSION_DURATION_MONITORING_INTERVAL_MAX_IN_MS)
            {
                interval = PVMF_JITTER_BUFFER_NODE_SESSION_DURATION_MONITORING_INTERVAL_MAX_IN_MS;
            }
            // <--- Demon Deng ALPS00139342 don't use this trick any more
#if 0
        // <--- Morris Yang ALPS00225250 Adjust iCurrentMonitoringIntervalInMS to correct value if we cost too much time on iCurrentMonitoringIntervalInMS (usually for high bit-rate content)
            if ((ipSessionDurationTimer->IsStartFromSetPlayRange()) && 
            	 (iLastBufferingTime >= 10000) &&
            	 (elapsedTime32 > iLastBufferingTime) &&
            	 (interval < (totalSessionDuration - (elapsedTime32 - iLastBufferingTime))))
            	{
            	     LOGD ("OOPS! Buffering cost too much time, adjust elapsed session duration");
            	     LOGD ("ComputeCurrentSessionDurationMonitoringInterval (Origin) - TotalDuration=%d, elapsedTime32=%d, ElapsedDuration=%d, CurrMonitoringInterval=%d", totalSessionDuration, elapsedTime32, elapsedSessionDuration, interval);
          	     ipSessionDurationTimer->SetElapsedSessionDuration(elapsedTime32 - iLastBufferingTime);
      	            interval = totalSessionDuration - ipSessionDurationTimer->GetElapsedSessionDurationInMS();
            	}
#endif
            // --->

            ipSessionDurationTimer->setCurrentMonitoringIntervalInMS(interval);

            //PVMF_JB_LOGCLOCK_SESSION_DURATION((0, "PVMFJBN::ComputeCurrentSessionDurationMonitoringInterval - TotalDuration=%d, ElapsedDuration=%d, CurrMonitoringInterval=%d", totalSessionDuration, elapsedSessionDuration, interval));
            LOGD ("ComputeCurrentSessionDurationMonitoringInterval - TotalDuration=%d, elapsedTime32=%d, ElapsedDuration=%d, CurrMonitoringInterval=%d", totalSessionDuration, elapsedTime32, ipSessionDurationTimer->GetElapsedSessionDurationInMS(), interval);
        // --->
            
        }

        PVMFMediaClock&     irClientPlaybackClock;      //Reference to the client playback clock passed on by its observer.
        PVMFJitterBufferMiscObserver*   ipObserver;
        Oscl_Vector<PVMFJitterBufferPortParams*, OsclMemAllocator>& irPortParamsQueue;

    // <--- Morris Yang ALPS00225250
        uint64 iLastBufferingStartTime;
        uint32 iLastBufferingTime;
    // --->

        Oscl_Vector<RTPSessionInfoForFirewallExchange, OsclMemAllocator> iRTPExchangeInfosForFirewallExchange;
        struct RateAdapatationInfo
        {
            RateAdapatationInfo(): iPort(NULL) {}
            PVMFPortInterface* iPort;
            RTPRateAdaptationInfo iRateAdapatationInfo;
        };
        Oscl_Vector<RateAdapatationInfo, OsclMemAllocator> iRateAdaptationInfos;

        struct RTCPParams
        {
            RTCPParams(): iFeedbackPort(NULL), iNumSenders(0), iRR(0), iRS(0) {}
            PVMFPortInterface* iFeedbackPort;
            int iNumSenders;
            uint32 iRR;
            uint32 iRS;
        };
        Oscl_Vector<RTCPParams, OsclMemAllocator> iRTCPParamsVect;

        //Sesion Info
        uint32  iSessionDuration;
        bool    iStreamingSessionExpired;
        bool    iPlayDurationAvailable;
        bool    iBroadcastSession;

        bool UseSessionDurationTimerForEOS();
        bool iUseSessionDurationTimerForEOS;

        uint32  iPlayStartTimeInMS;
        uint32  iPlayStopTimeInMS;
        bool    iPlayStopTimeAvailable;

        bool    iFireWallPacketsExchangeEnabled;

        bool   iEstimatedServerClockUpdateCallbackPending;
        uint32 iEstimatedServerClockUpdateCallbackId;

        PvmfJBSessionDurationTimer*     ipSessionDurationTimer;
        PVMFJBEventNotifier*            ipEventNotifier;
        PVRTCPProtoImplementor*         ipRTCPProtoImplementator;
        PVFirewallPacketExchangeImpl*   ipFireWallPacketExchangerImpl;

        PVMFMediaClock*     ipEstimatedServerClock;     //Estimation of the estimated server clock.
        PVMFMediaClock*     ipWallClock;                //A continously increasing clock.
        PVMFMediaClock*     ipNonDecreasingClock;       //Always increasing clock but may not be continous (may get pasued in between depending on the state of the streaming session)
        PVMFTimebase_Tickcount iEstimatedServerClockTimeBase;
        PVMFTimebase_Tickcount iWallClockTimeBase;
        PVMFTimebase_Tickcount iNonDecreasingTimeBase;

        bool iDisableRTCPFeedback;

        PVLogger* ipJBEventsClockLogger;
        PVLogger* ipRTCPDataPathLoggerIn;
        PVLogger* ipClockLoggerSessionDuration;
        PVLogger* ipClockLoggerRebuff;
        PVLogger* ipClockLogger;
        PVLogger* ipDataPathLoggerIn;
        PVLogger* ipDataPathLoggerOut;
        PVLogger* ipDataPathLoggerRTCP;
        PVLogger* ipLogger;
};

#endif
