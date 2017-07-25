#include "DaqDllEntityFactory.hpp"

#include "core/StaticComponentId.hpp"

#include "core/ManagerLocator.hpp"

#include "DaqDllEntityState.hpp"

namespace talorion {

DaqDllEntityFactory::DaqDllEntityFactory(QObject *par) : QObject(par)
{

}

DaqDllEntityFactory &DaqDllEntityFactory::instance()
{
    // Since it's a static variable, if the class has already been created,
    // It won't be created again.
    // And it **is** thread-safe in C++11.
    static DaqDllEntityFactory myInstance;
    // Return a reference to our instance.
    return myInstance;
}

QUuid DaqDllEntityFactory::daqDllEntityUid()
{
    return ("{91396a7c-6106-4580-9e72-8bb73f691fc8}");
}

EntityManager::ComponentID DaqDllEntityFactory::serialVersionUidComponentId()
{
    return SERIAL_VERSION_UID_COMPONENT;
}

EntityManager::ComponentID DaqDllEntityFactory::nameComponentId()
{
    return NAME_COMPONENT;
}

EntityManager::ComponentID DaqDllEntityFactory::dataAquisitionDllComponent()
{
    return DLL_PATH_COMPONENT;
}

EntityManager::ComponentID DaqDllEntityFactory::userDataUpdaterateComponent()
{
    return UPDATERATE_COMPONENT;
}

EntityManager::ComponentID DaqDllEntityFactory::timeoutComponent()
{
    return TIMEOUT_COMPONENT;
}

EntityManager::ComponentID DaqDllEntityFactory::userDataPathComponent()
{
    return USER_DATA_PATH_COMPONENT;
}

EntityManager::ComponentID DaqDllEntityFactory::daqDataIdComponent()
{
    return DAQ_DATA_ID_COMPONENT;
}

EntityManager::ComponentID DaqDllEntityFactory::stateComponent()
{
    return STATE_COMPONENT;
}

EntityManager::EntityID DaqDllEntityFactory::createNewDaqDllEntity(const QString &dll_name, const QString &dll_path, int timeout, const QString &usr_dta_path, int updaterate)
{
    EntityManager::EntityID daqDll = EntityManager::invalid_id;
    EntityManager& mng = ManagerLocator::entityManager();

    auto allDlls = allDaqDllEntities();
    if(allDlls.size() > 0){
        daqDll = allDlls.at(0);
        for(auto dll: allDlls){
            if(dll != daqDll){
                mng.removeEntity(dll);
            }
        }
    }else{
        daqDll       = mng.createNewEntity();

        mng.createComponentAndAddTo(nameComponentId(), daqDll);
        mng.createComponentAndAddTo(dataAquisitionDllComponent(), daqDll);
        mng.createComponentAndAddTo(timeoutComponent(), daqDll);
        mng.createComponentAndAddTo(userDataPathComponent(), daqDll);
        mng.createComponentAndAddTo(userDataUpdaterateComponent(), daqDll);
        mng.createComponentAndAddTo(daqDataIdComponent(), daqDll);
        mng.createComponentAndAddTo(serialVersionUidComponentId(), daqDll);
        //mng.createComponentAndAddTo(stateComponent(), daqDll);


        mng.setComponentDataForEntity(nameComponentId(),               daqDll, dll_name);
        mng.setComponentDataForEntity(dataAquisitionDllComponent(),   daqDll, dll_path);
        mng.setComponentDataForEntity(timeoutComponent(),   daqDll, timeout);
        mng.setComponentDataForEntity(userDataPathComponent(),   daqDll, usr_dta_path);
        mng.setComponentDataForEntity(userDataUpdaterateComponent(),   daqDll, updaterate);
        mng.setComponentDataForEntity(daqDataIdComponent(),   daqDll, EntityManager::invalid_id);
        //auto state = QVariant::fromValue(DaqDllEntityState::DAQ_DLL_STATE_UNKNOWN);
        //mng.setComponentDataForEntity(stateComponent(),   daqDll, state);
        auto uid= QVariant::fromValue(daqDllEntityUid());
        mng.setComponentDataForEntity(serialVersionUidComponentId(), daqDll, uid);
    }
    return daqDll;
}

EntityManager::EntityList DaqDllEntityFactory::allDaqDllEntities()
{
    EntityManager& mng = ManagerLocator::entityManager();

    auto uid= daqDllEntityUid();
    EntityManager::EntityList daqDlls;
    auto all_ents= mng.allEntities();
    for (auto entity: all_ents) {
        auto ent_uid = mng.componentDataForEntity(serialVersionUidComponentId(),entity).toUuid();
        if(ent_uid == uid){
            daqDlls<<entity;
        }
    }
    return daqDlls;
}

UserDataSourceList DaqDllEntityFactory::getUserDataComponents(EntityManager::EntityID daqDll)
{
    UserDataSourceList ret;

    EntityManager& mng = ManagerLocator::entityManager();
    if(mng.isValid(daqDll) == false)
        return ret;

    auto pathVar = mng.componentDataForEntity(userDataPathComponent(),daqDll);
    if(pathVar.isValid() == false)
        return ret;
    if(pathVar.canConvert<QString>() == false)
        return ret;
    auto path = pathVar.value<QString>();
    if(path.endsWith('/') == false)
        path += '/';

    EntityManager::ComponentID component_id;
    QVariant dataVar;
    double data;
    bool ok;
    QString  name;
    EntityManager::EntityID par;
    QString  parentName;
    QString  usrDataLocation;


    auto usrDataList = allEntitiesWithUserData();
    foreach (auto usrData, usrDataList) {
        component_id  = mng.get_userdata_component(usrData);
        dataVar       = mng.componentDataForEntity(component_id, usrData);
        name          = mng.get_name_component(usrData);
        if(name.isEmpty() || name.isNull())
            continue;
        if(dataVar.isValid() == false)
            continue;
        if(dataVar.canConvert<double>() == false)
            continue;
        data = dataVar.toDouble(&ok);
        if(ok == false)
            continue;

        usrDataLocation.clear();
        usrDataLocation += path;

        parentName.clear();
        par = mng.getParent(usrData);
        if(mng.isValid(par) == true){
            parentName = mng.get_ip_address_component(par);
            if(parentName.isEmpty() == false)
                usrDataLocation += parentName;
        }

        if(usrDataLocation.endsWith('/') == false)
            usrDataLocation += '/';


        usrDataLocation = usrDataLocation.normalized(QString::NormalizationForm_D);
        usrDataLocation.replace(".","_");

        QStringList descriptions{name};
        QVector<double> dta{data};
        UserDataSource src(usrDataLocation, descriptions, dta);
        //qDebug()<<usrDataLocation << name << data;

        auto index = ret.indexOf(src);
        if(index != -1){
            auto tmp = ret.at(index);
            tmp += src;
            ret.replace(index, tmp);
        }else{
            ret.append(src);
        }

    }
    return ret;
}

EntityManager::EntityList DaqDllEntityFactory::allEntitiesWithUserData()
{
    EntityManager& mng = ManagerLocator::entityManager();
    return mng.get_entities_with_userdata_components();
}

} // namespace talorion
