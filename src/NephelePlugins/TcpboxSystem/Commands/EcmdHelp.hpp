#ifndef TALORION_ECMD_HELP_HPP
#define TALORION_ECMD_HELP_HPP

#include "AbstractEcmdCommand.hpp"
#include <QTimer>



namespace talorion {

  class EcmdHelp : public AbstractEcmdCommand
  {
    Q_OBJECT
  public:
    explicit EcmdHelp(QObject *par = Q_NULLPTR);


    // abstract_ecmd_command interface
  public:
    virtual QString buildCommandString() const Q_DECL_OVERRIDE;

     virtual bool parseData(QByteArray &data) Q_DECL_OVERRIDE;

    // abstract_ecmd_command interface
  private:
    virtual quint64 doExpectedDataLength() const Q_DECL_OVERRIDE;


  };

} // namespace talorion

#endif // TALORION_ECMD_HELP_HPP
