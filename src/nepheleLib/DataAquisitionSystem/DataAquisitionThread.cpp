#include "DataAquisitionThread.hpp"
#include "DataAquisitionWorker.hpp"

#include <QThread>

#include <DataAquisitionSystem/DaqDllEntity/DaqDllEntityFactory.hpp>

#define tofdaqdll "C:\\Tofwerk\\TofDaq\\TofDaqDll.dll"

namespace talorion {

  DataAquisitionThread::DataAquisitionThread(QObject *par) :
    QThread(par),
    m_threadId(Q_NULLPTR),
    m_mutex()
  {

  }

  Qt::HANDLE DataAquisitionThread::threadId()
  {
    QMutexLocker locker(&m_mutex);

    if(m_threadId == Q_NULLPTR){
        return QThread::currentThreadId();
      }
    return m_threadId;
  }

  void DataAquisitionThread::run()
  {
    m_mutex.lock();
    auto old_thread_id = m_threadId;
    m_threadId = QThread::currentThreadId();

    DataAquisitionWorker wrk;
    auto dllid = DaqDllEntityFactory::createNewDaqDllEntity(QString("TofDaqDll.dll"), QString(tofdaqdll), 500, QString("/NEPHELE"), MIN_POLL_INTERVAL);
    wrk.connectToDaqDllEntity(dllid);
    //wrk.initAllDllEnities();

    m_mutex.unlock();

    exec();

    m_mutex.lock();
    m_threadId=(old_thread_id);

    //wrk.disposeAllDllEntities();
    wrk.disconnectToDaqDllEntity();

    m_mutex.unlock();
  }

} // namespace talorion
