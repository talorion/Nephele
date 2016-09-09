#ifndef ABSTRACT_ECMD_COMMAND_HPP
#define ABSTRACT_ECMD_COMMAND_HPP

#include <QObject>
namespace talorion {

  typedef enum ecmd_command_state{
    COMMAND_IDLE,
    COMMAND_SEND,
    COMMAND_RECEIVED,
    COMMAND_ERROR
  }ecmd_command_state_t;

  class abstract_ecmd_command : public QObject
  {
    Q_OBJECT
  public:
    explicit abstract_ecmd_command(const QString &cmd_str, QObject *par = Q_NULLPTR);

    QString command_string() const;

    virtual QString build_command_string()const=0;

  signals:

  public slots:

  protected:
    static const int MaxBufferSize;

    const QString m_command_string;
  };
}

#endif // ABSTRACT_ECMD_COMMAND_HPP
