#ifndef TALORION_QECMDCLIENT_HPP
#define TALORION_QECMDCLIENT_HPP

#include <QObject>
#include "qecmdclient_global.hpp"

namespace talorion {

  class QECMDCLIENTSHARED_EXPORT QEcmdClient :public QObject
  {

    Q_OBJECT
  public:
    //enum class QtAdsConnectionState : int {
    enum QEcmdClientState{
      IDLE,
      INITALIZED,
      CONNECTED,
      POLLING
    };
    Q_ENUM(QEcmdClientState)

  public:
    QEcmdClient();

    //void initialize();
    //void connectEcmd();
    //void startPolling();

    QtAdsConnectionState state() const;
  };


} // namespace talorion
#endif // TALORION_QECMDCLIENT_HPP
