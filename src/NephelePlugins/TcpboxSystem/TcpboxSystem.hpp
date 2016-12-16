#ifndef TALORION_TCPBOX_SYSTEM_HPP
#define TALORION_TCPBOX_SYSTEM_HPP

#include <QMap>
#include <QThread>

#include <tcpboxsystem_global.hpp>
#include "SystemManager/AbstractSystem.hpp"
#include "TcpboxFactory.hpp"
#include "TcpboxClient.hpp"
#include "TcpboxSystemThread.hpp"

namespace talorion {

  class TCPBOXSYSTEMSHARED_EXPORT TcpboxSystem : public AbstractSystem
  {
    Q_OBJECT
  public:
    friend class TcpboxFactory;

  public:
    using tcpbox_container = TcpboxFactory::tcpbox_container_t;
    using tcpbox_command_container = TcpboxFactory::tcpbox_command_container_t;

  public:
    explicit TcpboxSystem(QObject *par = Q_NULLPTR);
    virtual ~TcpboxSystem();

  signals:
    void newTcpbox(EntityManager::EntityID);
    void deleteTcpbox(EntityManager::EntityID);

  public:
    bool containsTcpbox(const tcpbox_container::value_type & tcpbox)const;
    void deleteBox(const tcpbox_container::value_type & tcpbox);
    void deleteAllBoxes();

    Qt::HANDLE threadId();

  public:
    tcpbox_container getConfiguredBoxes()const;

    // abstract_system interface
  private:
    virtual StateTransRet doInitialize() Q_DECL_OVERRIDE;
    virtual StateTransRet doDispose() Q_DECL_OVERRIDE;
    virtual StateTransRet doStart() Q_DECL_OVERRIDE;
    virtual QStringList doGetDefaultComponents() const Q_DECL_OVERRIDE;

  private:
    void addBox(const tcpbox_container::value_type & tcpbox);

  private:
    QPointer<TcpboxSystemThread> m_thread;


    // AbstractSystem interface
  private:
    virtual QString doSystemName() const Q_DECL_OVERRIDE;

  protected:
    virtual void doProvideEntityManager(EntityManager *service) Q_DECL_OVERRIDE;
    virtual void doProvideEventManager(EventManager *service) Q_DECL_OVERRIDE;
    virtual void doProvideSystemManager(SystemManager *service) Q_DECL_OVERRIDE;
  };

} // namespace talorion

#endif // TALORION_TCPBOX_SYSTEM_HPP
