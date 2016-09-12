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

  class abstract_ecmd_command : public QObject
  {
    Q_OBJECT
  public:
    explicit abstract_ecmd_command(const QString &cmd_str, QObject *par = Q_NULLPTR);

    QString command_string() const;

    virtual QString build_command_string()const=0;

    virtual bool parse_data(QByteArray& data)=0;

    quint64 expected_data_length()const;

    ecmd_command_state_t state() const;

    void setState(const ecmd_command_state_t &state);

  private:
    virtual quint64 do_expected_data_length()const=0;

  protected:
    static const int MaxBufferSize;

    const QString m_command_string;

    ecmd_command_state_t m_state;

  };
}

#endif // ABSTRACT_ECMD_COMMAND_HPP
