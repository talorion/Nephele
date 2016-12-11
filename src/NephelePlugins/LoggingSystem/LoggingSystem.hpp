#ifndef TALORION_LOGGING_SYSTEM_HPP
#define TALORION_LOGGING_SYSTEM_HPP

#include "SystemManager/AbstractSystem.hpp"

#include <QStringList>

namespace talorion {

  class LoggingSystem : public AbstractSystem
  {
    Q_OBJECT
  public:
    explicit LoggingSystem(QObject *par = Q_NULLPTR);

  public:
    QStringList getAllEntries()const;

    int maxNumEntries()const;
    void setMaxNumEntries( int mne);

    // abstract_system interface
  private:
    virtual StateTransRet doInitialize() Q_DECL_OVERRIDE;
    virtual StateTransRet doDispose() Q_DECL_OVERRIDE;
    virtual StateTransRet doStart() Q_DECL_OVERRIDE;

    static void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

  private:
    static QStringList s_entries;
    QtMessageHandler m_orig_msg_hdl;
    static  int s_max_num_entries;


    // AbstractSystem interface
  private:
    virtual QString doSystemName() const Q_DECL_OVERRIDE;

  protected:
    virtual void doProvideEntityManager(EntityManager *service) Q_DECL_OVERRIDE;
    virtual void doProvideEventManager(EventManager *service) Q_DECL_OVERRIDE;
    virtual void doProvideSystemManager(SystemManager *service) Q_DECL_OVERRIDE;
  };

} // namespace talorion

#endif // TALORION_LOGGING_SYSTEM_HPP
