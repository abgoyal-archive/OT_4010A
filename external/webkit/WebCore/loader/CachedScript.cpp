

#include "config.h"
#include "CachedScript.h"

#include "CachedResourceClient.h"
#include "CachedResourceClientWalker.h"
#include "TextResourceDecoder.h"
#include <wtf/Vector.h>

namespace WebCore {

CachedScript::CachedScript(const String& url, const String& charset)
    : CachedResource(url, Script)
    , m_decoder(TextResourceDecoder::create("application/javascript", charset))
    , m_decodedDataDeletionTimer(this, &CachedScript::decodedDataDeletionTimerFired)
{
    // It's javascript we want.
    // But some websites think their scripts are <some wrong mimetype here>
    // and refuse to serve them if we only accept application/x-javascript.
    setAccept("*/*");
}

CachedScript::~CachedScript()
{
}

void CachedScript::didAddClient(CachedResourceClient* c)
{
    if (!m_loading)
        c->notifyFinished(this);
}

void CachedScript::allClientsRemoved()
{
    m_decodedDataDeletionTimer.startOneShot(0);
}

void CachedScript::setEncoding(const String& chs)
{
    m_decoder->setEncoding(chs, TextResourceDecoder::EncodingFromHTTPHeader);
}

String CachedScript::encoding() const
{
    return m_decoder->encoding().name();
}

const String& CachedScript::script()
{
    ASSERT(!isPurgeable());

    if (!m_script && m_data) {
        m_script = m_decoder->decode(m_data->data(), encodedSize());
        m_script += m_decoder->flush();
        setDecodedSize(m_script.length() * sizeof(UChar));
    }

    m_decodedDataDeletionTimer.startOneShot(0);
    return m_script;
}

void CachedScript::data(PassRefPtr<SharedBuffer> data, bool allDataReceived)
{
    if (!allDataReceived)
        return;

    m_data = data;
    setEncodedSize(m_data.get() ? m_data->size() : 0);
    m_loading = false;
    checkNotify();
}

void CachedScript::checkNotify()
{
    if (m_loading)
        return;

    CachedResourceClientWalker w(m_clients);
    while (CachedResourceClient* c = w.next())
        c->notifyFinished(this);
}

void CachedScript::error()
{
    m_loading = false;
    m_errorOccurred = true;
    checkNotify();
}

void CachedScript::destroyDecodedData()
{
    m_script = String();
    setDecodedSize(0);
    if (isSafeToMakePurgeable())
        makePurgeable(true);
}

void CachedScript::decodedDataDeletionTimerFired(Timer<CachedScript>*)
{
    destroyDecodedData();
}

} // namespace WebCore
