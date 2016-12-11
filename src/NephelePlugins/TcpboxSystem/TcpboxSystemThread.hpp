#ifndef TALORION_TCPBOX_SYSTEM_THREAD_HPP
#define TALORION_TCPBOX_SYSTEM_THREAD_HPP

#include <QMutex>
#include <QThread>

#include "TcpboxFactory.hpp"
#include "TcpboxClient.hpp"
#include "EcmdConnection/EcmdConnectionManager.hpp"

namespace talorion {

  class TcpboxSystem;

  class TcpboxSystemThread : public QThread
  {
    Q_OBJECT
  public:
    friend class TcpboxFactory;

  public:
    explicit TcpboxSystemThread(const TcpboxSystem &sys, QObject *par = Q_NULLPTR);
    virtual ~TcpboxSystemThread();

  private:
      Q_DISABLE_COPY(TcpboxSystemThread)

  public:
    Qt::HANDLE threadId();

  private:
    const TcpboxSystem& m_sys;
    Qt::HANDLE m_thread_id;
    QMutex m_mutex;

    // QThread interface
  protected:
    virtual void run() Q_DECL_OVERRIDE;
  };

} // namespace talorion

#endif // TALORION_TCPBOX_SYSTEM_THREAD_HPP
