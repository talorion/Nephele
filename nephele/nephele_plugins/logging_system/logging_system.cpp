#include "logging_system.hpp"

#include <QDateTime>
#include <QDebug>

namespace talorion {

  QStringList logging_system::s_entries= QStringList();
  int logging_system::s_max_num_entries= 100;

  logging_system::logging_system(QObject *par) :
    abstract_system(par),
    m_orig_msg_hdl(nullptr)
  {

  }

  logging_system::~logging_system()
  {

  }

  QStringList logging_system::get_all_entries() const
  {
    return s_entries;
  }

  int logging_system::max_num_entries() const
  {
    return s_max_num_entries;
  }

  void logging_system::set_max_num_entries( int mne)
  {
    if(mne>0){
        s_max_num_entries=mne;
        qDebug()<<"changed max number of log messages to"<<mne;
      }
  }

  abstract_system::state_trans_ret_t logging_system::do_initialize()
  {
    //To restore the message handler, call qInstallMessageHandler(0).
    m_orig_msg_hdl= qInstallMessageHandler(logging_system::myMessageOutput);
    return 0;
  }

  abstract_system::state_trans_ret_t logging_system::do_dispose()
  {
    qInstallMessageHandler(m_orig_msg_hdl);
    s_entries.clear();
    return 0;
  }

  abstract_system::state_trans_ret_t logging_system::do_start()
  {
    return 0;
  }


  void logging_system::myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
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

} // namespace talorion
