#include "GuiSystem.hpp"

#include <ManagerLocator.hpp>
#include <QApplication>
#include <QDebug>
#include <Style.hpp>

#include <NepheleMainWindow/NepheleMainWindow.hpp>


namespace talorion {

  GuiSystem::GuiSystem(QObject *par):
    AbstractSystem(par),
    m_main_window(Q_NULLPTR)
  {

  }

  GuiSystem::~GuiSystem()
  {
    if(m_main_window != Q_NULLPTR)
      delete m_main_window;
  }

  AbstractSystem::StateTransRet GuiSystem::doInitialize()
  {
    qApp->setStyleSheet(Style::getFlatStyle());
    return 0;
  }

  AbstractSystem::StateTransRet GuiSystem::doDispose()
  {
    return 0;
  }

  AbstractSystem::StateTransRet GuiSystem::doStart()
  {
    m_main_window= new NepheleMainWindow();

    m_main_window->resize(1024,768);
    //m_main_window->readSettings();

    m_main_window->show();

    return 0;
  }

  QString GuiSystem::doSystemName() const
  {
    return QString("GuiSystem");
  }

  void GuiSystem::doProvideEntityManager(EntityManager *service)
  {
    ManagerLocator::provideEntityManager(service);
  }

  void GuiSystem::doProvideEventManager(EventManager *service)
  {
    ManagerLocator::provideEventManager(service);
  }

  void GuiSystem::doProvideSystemManager(SystemManager *service)
  {
    ManagerLocator::provideSystemManager(service);
  }


} // namespace talorion
