#ifndef ABSTRACT_ECMD_COMMAND_HPP
#define ABSTRACT_ECMD_COMMAND_HPP

#include <QObject>
#include <QAbstractSocket>
#include <QTimer>

namespace talorion {

  typedef enum ecmd_command_state{
    COMMAND_STATE_UNKNOWN,
    COMMAND_STATE_STARTED,
    COMMAND_STATE_ERROR,
    COMMAND_STATE_OK
  }ecmd_command_state_t;

  class AbstractEcmdCommand : public QObject
  {
    Q_OBJECT
  public:
    explicit AbstractEcmdCommand(const QString &cmd_str, QObject *par = Q_NULLPTR);

    QString commandString() const;

    virtual QString buildCommandString()const=0;

    virtual bool parseData(QByteArray& data)=0;

    quint64 expectedDataLength()const;

    ecmd_command_state_t state() const;

    void setState(const ecmd_command_state_t &state);

  private:
    virtual quint64 doExpectedDataLength()const=0;

  protected:
    static const int MaxBufferSize;

    const QString m_command_string;

    ecmd_command_state_t m_state;

  };
}

#endif // ABSTRACT_ECMD_COMMAND_HPP
