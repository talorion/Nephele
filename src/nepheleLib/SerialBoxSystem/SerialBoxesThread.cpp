#include "SerialBoxesThread.hpp"
#include "SerialBoxesWorker.hpp"

namespace talorion {

SerialBoxesThread::SerialBoxesThread(QObject *par):
    QThread(par)
{

}

void SerialBoxesThread::run()
{
    m_mutex.lock();
    //auto old_thread_id = m_threadId;
    //m_threadId = QThread::currentThreadId();

    SerialBoxesWorker wrk;
    //auto dllid = DaqDllEntityFactory::createNewDaqDllEntity(QString("TofDaqDll.dll"), QString(tofdaqdll), 500, QString("/NEPHELE"), MIN_POLL_INTERVAL);
    //wrk.connectToDaqDllEntity(dllid);
    m_mutex.unlock();

    exec();

    m_mutex.lock();
    //m_threadId=(old_thread_id);
    //wrk.disconnectToDaqDllEntity();

    m_mutex.unlock();
}

} // namespace talorion
