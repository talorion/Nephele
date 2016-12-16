#include <QApplication>

#include <ManagerLocator.hpp>
#include <QtPlugin>
using namespace talorion;

//#include <SystemManager/AbstractSystem.hpp>
//Q_IMPORT_PLUGIN(AbstractSystem)

#include <GuiSystem.hpp>
//Q_IMPORT_PLUGIN(GuiSystem)

int main(int argc, char *argv[]){

  QApplication* a = new QApplication(argc, argv);

  QString v = "0.1.0.0";
  QCoreApplication::setOrganizationName("Nephele");
  QCoreApplication::setOrganizationDomain("github.com/talorion");
  QCoreApplication::setApplicationName("Nephele");
  QCoreApplication::setApplicationVersion(v);

  ManagerLocator::initialize();
  ManagerLocator::provideEntityManager(new EntityManager(a));
  ManagerLocator::provideEventManager(new EventManager(a));
  ManagerLocator::provideSystemManager(new SystemManager(a));

  //load Systems
  ManagerLocator::systemManager().registerNewSystem<GuiSystem>();
  //ManagerLocator::systemManager().loadPluginSystems();

  ManagerLocator::systemManager().startAllSystems();

  int ret = 0;
  ret =a->exec();

  ManagerLocator::systemManager().disposeAllSystems();
  ManagerLocator::systemManager().deleteAllSystems();

  ManagerLocator::dispose();

  delete a;

  return ret;
}
