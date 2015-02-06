

#include "config.h"
#include "ScriptExecutionContext.h"

#include "ActiveDOMObject.h"
#include "Database.h"
#include "DatabaseTask.h"
#include "DatabaseThread.h"
#include "MessagePort.h"
#include "SecurityOrigin.h"
#include "WorkerContext.h"
#include "WorkerThread.h"
#include <wtf/MainThread.h>
#include <wtf/PassRefPtr.h>

#if USE(JSC)
#include "JSDOMWindow.h"
#endif

namespace WebCore {

class ProcessMessagesSoonTask : public ScriptExecutionContext::Task {
public:
    static PassOwnPtr<ProcessMessagesSoonTask> create()
    {
        return new ProcessMessagesSoonTask;
    }

    virtual void performTask(ScriptExecutionContext* context)
    {
        context->dispatchMessagePortEvents();
    }
};

ScriptExecutionContext::ScriptExecutionContext()
#if ENABLE(DATABASE)
    : m_hasOpenDatabases(false)
#endif
{
}

ScriptExecutionContext::~ScriptExecutionContext()
{
    HashMap<ActiveDOMObject*, void*>::iterator activeObjectsEnd = m_activeDOMObjects.end();
    for (HashMap<ActiveDOMObject*, void*>::iterator iter = m_activeDOMObjects.begin(); iter != activeObjectsEnd; ++iter) {
        ASSERT(iter->first->scriptExecutionContext() == this);
        iter->first->contextDestroyed();
    }

    HashSet<MessagePort*>::iterator messagePortsEnd = m_messagePorts.end();
    for (HashSet<MessagePort*>::iterator iter = m_messagePorts.begin(); iter != messagePortsEnd; ++iter) {
        ASSERT((*iter)->scriptExecutionContext() == this);
        (*iter)->contextDestroyed();
    }
#if ENABLE(DATABASE)
    if (m_databaseThread) {
        ASSERT(m_databaseThread->terminationRequested());
        m_databaseThread = 0;
    }
#endif
}

#if ENABLE(DATABASE)

DatabaseThread* ScriptExecutionContext::databaseThread()
{
    if (!m_databaseThread && !m_hasOpenDatabases) {
        // Create the database thread on first request - but not if at least one database was already opened,
        // because in that case we already had a database thread and terminated it and should not create another.
        m_databaseThread = DatabaseThread::create();
        if (!m_databaseThread->start())
            m_databaseThread = 0;
    }

    return m_databaseThread.get();
}

void ScriptExecutionContext::addOpenDatabase(Database* database)
{
    ASSERT(isContextThread());
    if (!m_openDatabaseSet)
        m_openDatabaseSet.set(new DatabaseSet());

    ASSERT(!m_openDatabaseSet->contains(database));
    m_openDatabaseSet->add(database);
}

void ScriptExecutionContext::removeOpenDatabase(Database* database)
{
    ASSERT(isContextThread());
    ASSERT(m_openDatabaseSet && m_openDatabaseSet->contains(database));
    if (!m_openDatabaseSet)
        return;
    m_openDatabaseSet->remove(database);
}

void ScriptExecutionContext::stopDatabases(DatabaseTaskSynchronizer* cleanupSync)
{
    ASSERT(isContextThread());
    if (m_openDatabaseSet) {
        DatabaseSet::iterator i = m_openDatabaseSet->begin();
        DatabaseSet::iterator end = m_openDatabaseSet->end();
        for (; i != end; ++i) {
            (*i)->stop();
            if (m_databaseThread)
                m_databaseThread->unscheduleDatabaseTasks(*i);
        }
    }
    
    if (m_databaseThread)
        m_databaseThread->requestTermination(cleanupSync);
    else if (cleanupSync)
        cleanupSync->taskCompleted();
}

#endif

void ScriptExecutionContext::processMessagePortMessagesSoon()
{
    postTask(ProcessMessagesSoonTask::create());
}

void ScriptExecutionContext::dispatchMessagePortEvents()
{
    RefPtr<ScriptExecutionContext> protect(this);

    // Make a frozen copy.
    Vector<MessagePort*> ports;
    copyToVector(m_messagePorts, ports);

    unsigned portCount = ports.size();
    for (unsigned i = 0; i < portCount; ++i) {
        MessagePort* port = ports[i];
        // The port may be destroyed, and another one created at the same address, but this is safe, as the worst that can happen
        // as a result is that dispatchMessages() will be called needlessly.
        if (m_messagePorts.contains(port) && port->started())
            port->dispatchMessages();
    }
}

void ScriptExecutionContext::createdMessagePort(MessagePort* port)
{
    ASSERT(port);
#if ENABLE(WORKERS)
    ASSERT((isDocument() && isMainThread())
        || (isWorkerContext() && currentThread() == static_cast<WorkerContext*>(this)->thread()->threadID()));
#endif

    m_messagePorts.add(port);
}

void ScriptExecutionContext::destroyedMessagePort(MessagePort* port)
{
    ASSERT(port);
#if ENABLE(WORKERS)
    ASSERT((isDocument() && isMainThread())
        || (isWorkerContext() && currentThread() == static_cast<WorkerContext*>(this)->thread()->threadID()));
#endif

    m_messagePorts.remove(port);
}

bool ScriptExecutionContext::canSuspendActiveDOMObjects()
{
    // No protection against m_activeDOMObjects changing during iteration: canSuspend() shouldn't execute arbitrary JS.
    HashMap<ActiveDOMObject*, void*>::iterator activeObjectsEnd = m_activeDOMObjects.end();
    for (HashMap<ActiveDOMObject*, void*>::iterator iter = m_activeDOMObjects.begin(); iter != activeObjectsEnd; ++iter) {
        ASSERT(iter->first->scriptExecutionContext() == this);
        if (!iter->first->canSuspend())
            return false;
    }
    return true;
}

void ScriptExecutionContext::suspendActiveDOMObjects()
{
    // No protection against m_activeDOMObjects changing during iteration: suspend() shouldn't execute arbitrary JS.
    HashMap<ActiveDOMObject*, void*>::iterator activeObjectsEnd = m_activeDOMObjects.end();
    for (HashMap<ActiveDOMObject*, void*>::iterator iter = m_activeDOMObjects.begin(); iter != activeObjectsEnd; ++iter) {
        ASSERT(iter->first->scriptExecutionContext() == this);
        iter->first->suspend();
    }
}

void ScriptExecutionContext::resumeActiveDOMObjects()
{
    // No protection against m_activeDOMObjects changing during iteration: resume() shouldn't execute arbitrary JS.
    HashMap<ActiveDOMObject*, void*>::iterator activeObjectsEnd = m_activeDOMObjects.end();
    for (HashMap<ActiveDOMObject*, void*>::iterator iter = m_activeDOMObjects.begin(); iter != activeObjectsEnd; ++iter) {
        ASSERT(iter->first->scriptExecutionContext() == this);
        iter->first->resume();
    }
}

void ScriptExecutionContext::stopActiveDOMObjects()
{
    // No protection against m_activeDOMObjects changing during iteration: stop() shouldn't execute arbitrary JS.
    HashMap<ActiveDOMObject*, void*>::iterator activeObjectsEnd = m_activeDOMObjects.end();
    for (HashMap<ActiveDOMObject*, void*>::iterator iter = m_activeDOMObjects.begin(); iter != activeObjectsEnd; ++iter) {
        ASSERT(iter->first->scriptExecutionContext() == this);
        iter->first->stop();
    }
}

void ScriptExecutionContext::createdActiveDOMObject(ActiveDOMObject* object, void* upcastPointer)
{
    ASSERT(object);
    ASSERT(upcastPointer);
    m_activeDOMObjects.add(object, upcastPointer);
}

void ScriptExecutionContext::destroyedActiveDOMObject(ActiveDOMObject* object)
{
    ASSERT(object);
    m_activeDOMObjects.remove(object);
}

void ScriptExecutionContext::setSecurityOrigin(PassRefPtr<SecurityOrigin> securityOrigin)
{
    m_securityOrigin = securityOrigin;
}

void ScriptExecutionContext::addTimeout(int timeoutId, DOMTimer* timer)
{
    ASSERT(!m_timeouts.contains(timeoutId));
    m_timeouts.set(timeoutId, timer);
}

void ScriptExecutionContext::removeTimeout(int timeoutId)
{
    m_timeouts.remove(timeoutId);
}

DOMTimer* ScriptExecutionContext::findTimeout(int timeoutId)
{
    return m_timeouts.get(timeoutId);
}

ScriptExecutionContext::Task::~Task()
{
}

#if USE(JSC)
JSC::JSGlobalData* ScriptExecutionContext::globalData()
{
     if (isDocument())
        return JSDOMWindow::commonJSGlobalData();

#if ENABLE(WORKERS)
    if (isWorkerContext())
        return static_cast<WorkerContext*>(this)->script()->globalData();
#endif

    ASSERT_NOT_REACHED();
    return 0;
}
#endif

} // namespace WebCore
