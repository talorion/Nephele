#include <QApplication>

#include <ManagerLocator.hpp>
#include <GuiSystem.hpp>

int main(int argc, char *argv[]){

  QApplication* a = new QApplication(argc, argv);

  QString v = "0.1.0.0";
  QCoreApplication::setOrganizationName("Nephele");
  QCoreApplication::setOrganizationDomain("github.com/talorion");
  QCoreApplication::setApplicationName("Nephele");
  QCoreApplication::setApplicationVersion(v);

  talorion::ManagerLocator::initialize();
  talorion::ManagerLocator::provideEntityManager(new talorion::EntityManager(a));
  talorion::ManagerLocator::provideEventManager(new talorion::EventManager(a));
  talorion::ManagerLocator::provideSystemManager(new talorion::SystemManager(a));

  //load Systems
  talorion::ManagerLocator::systemManager().registerNewSystem<talorion::GuiSystem>();
  //talorion::ManagerLocator::systemManager().loadPluginSystems();

  talorion::ManagerLocator::systemManager().startAllSystems();

  int ret = 0;
  ret =a->exec();

  talorion::ManagerLocator::systemManager().disposeAllSystems();
  talorion::ManagerLocator::systemManager().deleteAllSystems();

  talorion::ManagerLocator::dispose();

  delete a;

  return ret;
}
