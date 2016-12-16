#ifndef TALORION_GUISYSTEM_HPP
#define TALORION_GUISYSTEM_HPP

#include "guisystem_global.hpp"

#include "SystemManager/AbstractSystem.hpp"

namespace talorion {

  class NepheleMainWindow;

  class GUISYSTEMSHARED_EXPORT GuiSystem : public AbstractSystem
  {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.nephele.core.AbstractSystem" FILE "GuiSystem.json")
    Q_INTERFACES(talorion::AbstractSystem)

  public:
    explicit GuiSystem(QObject *par = Q_NULLPTR);
    virtual ~GuiSystem();

  private:
      Q_DISABLE_COPY(GuiSystem)

    // AbstractSystem interface
  private:
    virtual StateTransRet doInitialize() Q_DECL_OVERRIDE;
    virtual StateTransRet doDispose() Q_DECL_OVERRIDE;
    virtual StateTransRet doStart() Q_DECL_OVERRIDE;
    virtual QString doSystemName() const Q_DECL_OVERRIDE;

  protected:
    virtual void doProvideEntityManager(EntityManager *service) Q_DECL_OVERRIDE;
    virtual void doProvideEventManager(EventManager *service) Q_DECL_OVERRIDE;
    virtual void doProvideSystemManager(SystemManager *service) Q_DECL_OVERRIDE;

  private:
    NepheleMainWindow* m_main_window;
  };


} // namespace talorion
#endif // TALORION_GUISYSTEM_HPP
