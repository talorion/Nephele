#ifndef ABSTRACT_ECMD_COMMAND_HPP
#define ABSTRACT_ECMD_COMMAND_HPP

#include <QObject>
namespace talorion {

  class abstract_ecmd_command : public QObject
  {
    Q_OBJECT
  public:
    explicit abstract_ecmd_command(const QString &cmd_str, QObject *par = Q_NULLPTR);

    QString command_string() const;

  signals:

  public slots:

  protected:
    static const int MaxBufferSize;

    const QString m_command_string;
  };
}

#endif // ABSTRACT_ECMD_COMMAND_HPP
