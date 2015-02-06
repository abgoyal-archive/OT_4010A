
#ifndef SDP_MEDIAPARSER_FACTORY_H
#define SDP_MEDIAPARSER_FACTORY_H
#include "oscl_types.h"
#include "sdp_mediaparser_registry.h"

class SDPAMRMediaParserFactory : public SDPMediaParserFactory
{
    public:
        virtual ~SDPAMRMediaParserFactory() {}
    private:
        virtual SDPBaseMediaInfoParser* createSDPMediaParserInstance();
        virtual void destruct_and_dealloc(OsclAny* ptr);
};

class SDPAACMediaParserFactory : public SDPMediaParserFactory
{
    public:
        virtual ~SDPAACMediaParserFactory() {}
    private:
        virtual SDPBaseMediaInfoParser* createSDPMediaParserInstance();
        virtual void destruct_and_dealloc(OsclAny* ptr);
};

class SDPH263MediaParserFactory : public SDPMediaParserFactory
{
    public:
        virtual ~SDPH263MediaParserFactory() {}
    private:
        virtual SDPBaseMediaInfoParser* createSDPMediaParserInstance();
        virtual void destruct_and_dealloc(OsclAny* ptr);
};

class SDPH264MediaParserFactory : public SDPMediaParserFactory
{
    public:
        virtual ~SDPH264MediaParserFactory() {}
    private:
        virtual SDPBaseMediaInfoParser* createSDPMediaParserInstance();
        virtual void destruct_and_dealloc(OsclAny* ptr);
};

class SDPMPEG4MediaParserFactory : public SDPMediaParserFactory
{
    public:
        virtual ~SDPMPEG4MediaParserFactory() {}
    private:
        virtual SDPBaseMediaInfoParser* createSDPMediaParserInstance();
        virtual void destruct_and_dealloc(OsclAny* ptr);
};

class SDPRFC3640MediaParserFactory : public SDPMediaParserFactory
{
        virtual SDPBaseMediaInfoParser* createSDPMediaParserInstance();
        virtual void destruct_and_dealloc(OsclAny* ptr);
};

class SDPStillImageMediaParserFactory : public SDPMediaParserFactory
{
    public:
        virtual ~SDPStillImageMediaParserFactory() {}
    private:
        virtual SDPBaseMediaInfoParser* createSDPMediaParserInstance();
        virtual void destruct_and_dealloc(OsclAny* ptr);
};

class SDPPCMAMediaParserFactory : public SDPMediaParserFactory
{
    public:
        virtual ~SDPPCMAMediaParserFactory() {}
    private:
        virtual SDPBaseMediaInfoParser* createSDPMediaParserInstance();
        virtual void destruct_and_dealloc(OsclAny* ptr);
};

class SDPPCMUMediaParserFactory : public SDPMediaParserFactory
{
    public:
        virtual ~SDPPCMUMediaParserFactory() {}
    private:
        virtual SDPBaseMediaInfoParser* createSDPMediaParserInstance();
        virtual void destruct_and_dealloc(OsclAny* ptr);
};

#endif //SDP_MEDIAPARSER_FACTORY_H
