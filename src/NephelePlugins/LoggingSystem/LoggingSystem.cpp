#include "LoggingSystem.hpp"

#include <ManagerLocator.hpp>
#include <QDateTime>
#include <QDebug>

namespace talorion {

  QStringList LoggingSystem::s_entries= QStringList();
  int LoggingSystem::s_max_num_entries= 100;

  LoggingSystem::LoggingSystem(QObject *par) :
    AbstractSystem(par),
    m_orig_msg_hdl(0)
  {

  }

  QStringList LoggingSystem::getAllEntries() const
  {
    return s_entries;
  }

  int LoggingSystem::maxNumEntries() const
  {
    return s_max_num_entries;
  }

  void LoggingSystem::setMaxNumEntries( int mne)
  {
    if(mne>0){
        s_max_num_entries=mne;
        qDebug()<<"changed max number of log messages to"<<mne;
      }
  }

  AbstractSystem::StateTransRet LoggingSystem::doInitialize()
  {
    //To restore the message handler, call qInstallMessageHandler(0).
    m_orig_msg_hdl= qInstallMessageHandler(LoggingSystem::myMessageOutput);
    return 0;
  }

  AbstractSystem::StateTransRet LoggingSystem::doDispose()
  {
    qInstallMessageHandler(m_orig_msg_hdl);
    s_entries.clear();
    return 0;
  }

  AbstractSystem::StateTransRet LoggingSystem::doStart()
  {
    return 0;
  }


  void LoggingSystem::myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
  {
    Q_UNUSED(context)

    QDateTime now=QDateTime::currentDateTime();
    QString timestring="["+now.toString(Qt::ISODate)+"]";

    QString severity_string;
    switch (type) {
      case QtDebugMsg:
        severity_string="DEBUG";
        break;
      case QtInfoMsg:
        severity_string="INFO";
        break;
      case QtWarningMsg:
        severity_string="WARNING";
        break;
      case QtCriticalMsg:
        severity_string="CRITICAL";
        break;
      case QtFatalMsg:
        severity_string="FATAL";
        break;
      }

    QString enh_msg=timestring+"\t"+severity_string+"\t"+msg;

    while(s_entries.size() >= s_max_num_entries)
      s_entries.pop_front();
    s_entries<<msg;

    QByteArray enh_msg_raw=enh_msg.toLocal8Bit();
    fprintf(stderr, "%s\n", enh_msg_raw.constData());

    switch (type) {
      case QtFatalMsg:
        abort();
      default:
        return;
      }
  }

  QString LoggingSystem::doSystemName() const
  {
    return QString("Logging System");
  }

  void LoggingSystem::doProvideEntityManager(EntityManager *service)
  {
    ManagerLocator::provideEntityManager(service);
  }

  void LoggingSystem::doProvideEventManager(EventManager *service)
  {
    ManagerLocator::provideEventManager(service);
  }

  void LoggingSystem::doProvideSystemManager(SystemManager *service)
  {
    ManagerLocator::provideSystemManager(service);
  }

} // namespace talorion
