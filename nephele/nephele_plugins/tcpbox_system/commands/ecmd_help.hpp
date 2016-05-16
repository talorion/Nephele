#ifndef TALORION_ECMD_HELP_HPP
#define TALORION_ECMD_HELP_HPP

#include "abstract_ecmd_command.hpp"
#include <QTimer>



namespace talorion {

  class ecmd_help : public abstract_ecmd_command
  {
    Q_OBJECT
  public:
    explicit ecmd_help(QObject *par = Q_NULLPTR);

    //bool send_command(const QString &message);

  signals:

  public slots:

  private slots:
    //void process_ready_read();
    //void sendGreetingMessage();

    // QObject interface
  protected:
    //virtual void timerEvent(QTimerEvent *) Q_DECL_OVERRIDE;

  private:
    //int readDataIntoBuffer(int maxSize = MaxBufferSize);
    //int dataLengthForCurrentDataType();
    //bool readProtocolHeader();
    //bool hasEnoughData();
    //void processData();

  private:
    //QString m_command;
    //QTimer m_send_timer;
    //QTime m_receive_time;
    //QByteArray m_buffer;
    //ConnectionState state;
    //DataType currentDataType;
    //int numBytesForCurrentDataType;
    //int m_transfer_timer_id;


  };

} // namespace talorion

#endif // TALORION_ECMD_HELP_HPP
