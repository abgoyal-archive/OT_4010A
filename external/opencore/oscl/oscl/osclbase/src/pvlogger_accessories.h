
#ifndef PVLOGGER_ACCESSORIES_H_INCLUDED
#define PVLOGGER_ACCESSORIES_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef PVLOGGER_H_INCLUDED
#include "pvlogger.h"
#endif

class PVLoggerLayout
{
    public:
        typedef PVLogger::message_id_type message_id_type;

        virtual ~PVLoggerLayout() {}

        /**
         * Formats the string and copies it to the given buffer.
         *
         * @return The length of the string not including the trailing '\0'
         */
        virtual int32 FormatString(char* formatBuf, int32 formatBufSize,
                                   message_id_type msgID, const char * fmt,
                                   va_list va) = 0;

        /**
         * Formats the data and copies it to the given buffer.
         *
         * @return The length of the buffer used.
         */
        virtual int32 FormatOpaqueMessage(char* formatBuf, int32 formatBufSize,
                                          message_id_type msgID, int32 numPairs,
                                          va_list va) = 0;
};

class PVLoggerFilter
{
    public:
        virtual ~PVLoggerFilter() {}

        typedef PVLogger::message_id_type message_id_type;
        typedef PVLogger::log_level_type log_level_type;
        typedef PVLogger::filter_status_type filter_status_type;

        virtual filter_status_type FilterString(char* tag, message_id_type msgID, log_level_type level) = 0;
        virtual filter_status_type FilterOpaqueMessge(char* tag, message_id_type msgID, log_level_type level) = 0;
};

const PVLoggerFilter::filter_status_type PVLOGGER_FILTER_ACCEPT = 1;
const PVLoggerFilter::filter_status_type PVLOGGER_FILTER_REJECT = 2;
const PVLoggerFilter::filter_status_type PVLOGGER_FILTER_NEUTRAL = 3;

class AllPassFilter : public PVLoggerFilter
{
    public:
        typedef PVLoggerFilter::message_id_type message_id_type;
        typedef PVLoggerFilter::log_level_type log_level_type;
        typedef PVLoggerFilter::filter_status_type filter_status_type;

        AllPassFilter() {};
        virtual ~AllPassFilter() {};

        filter_status_type FilterString(char* tag, message_id_type msgID, log_level_type level)
        {
            OSCL_UNUSED_ARG(tag);
            OSCL_UNUSED_ARG(msgID);
            OSCL_UNUSED_ARG(level);
            return (PVLOGGER_FILTER_ACCEPT);
        };
        filter_status_type FilterOpaqueMessge(char* tag, message_id_type msgID, log_level_type level)
        {
            OSCL_UNUSED_ARG(tag);
            OSCL_UNUSED_ARG(msgID);
            OSCL_UNUSED_ARG(level);
            return (PVLOGGER_FILTER_ACCEPT);
        };
};

class PVLoggerAppender
{
    public:
        typedef PVLogger::message_id_type message_id_type;

        virtual ~PVLoggerAppender() {}

        virtual void AppendString(message_id_type msgID, const char *fmt, va_list va) = 0;
        virtual void AppendBuffers(message_id_type msgID, int32 numPairs, va_list va) = 0;
};


#endif

