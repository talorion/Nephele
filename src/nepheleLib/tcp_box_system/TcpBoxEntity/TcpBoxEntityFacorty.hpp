#ifndef TCPBOXENTITYFACORTY_HPP
#define TCPBOXENTITYFACORTY_HPP

#include <QObject>

#include "core/EntityManager.hpp"
#include "core/ManagerLocator.hpp"
#include "core/StaticComponentId.hpp"
#include "../JsonRpc/jsonrpcmethoddescriptor.hpp"

namespace talorion {

enum ValueType {
  VT_DIGITAL,
  VT_ANALOG,
  VT_NONE
};
enum ValueMode {
  VM_INPUT = 0x0001,
  VM_OUTPUT = 0x0002,
  VM_INPUTOUTPUT = VM_INPUT | VM_OUTPUT,
  VM_NONE
};

class TcpBoxEntityFacorty : public QObject
{
    Q_OBJECT
private:
    explicit TcpBoxEntityFacorty(QObject *par = Q_NULLPTR);

public:
    static static_component_id nameComponentId(){return NAME_COMPONENT;}
    static static_component_id boxIdComponentId(){return BOX_ID_COMPONENT;}
    static static_component_id ipAddressComponentId(){return IP_ADDRESS_COMPONENT;}
    static static_component_id portComponentId(){return PORT_COMPONENT;}
    static static_component_id connectionStateComponentId(){return CONNECTION_STATE_COMPONENT;}
    static static_component_id backendComponentId(){return TCP_BOX_BACKEND_COMPONENT;}

public:
    static bool isConfigured(EntityManager::EntityID box);
    static QList<EntityManager::EntityID> getMethodsOfBox(EntityManager::EntityID box);
    static QList<EntityManager::EntityID> getValuesOfBox(EntityManager::EntityID box);

    static void addMethodToBox(EntityManager::EntityID box, EntityManager::EntityID meth_id);

    static EntityManager::EntityID getAOValue(EntityManager::EntityID box = EntityManager::invalid_id, int valid = -1);
    static EntityManager::EntityID getAIValue(EntityManager::EntityID box = EntityManager::invalid_id, int valid = -1);
    static EntityManager::EntityID getAIOValue(EntityManager::EntityID box = EntityManager::invalid_id, int valid = -1);

    static EntityManager::EntityID getDOValue(EntityManager::EntityID box = EntityManager::invalid_id, int valid = -1);
    static EntityManager::EntityID getDIValue(EntityManager::EntityID box = EntityManager::invalid_id, int valid = -1);
    static EntityManager::EntityID getDIOValue(EntityManager::EntityID box = EntityManager::invalid_id, int valid = -1);

    static EntityManager::EntityID getValue(EntityManager::EntityID box = EntityManager::invalid_id, int valid = -1);

    static ValueType getValueType(EntityManager::EntityID val);
    static ValueMode getValueMode(EntityManager::EntityID val);


    static bool isValue(EntityManager::EntityID entity);

private:
    static EntityManager& emngr(){return ManagerLocator::entityManager();}
};

} // namespace talorion

#endif // TCPBOXENTITYFACORTY_HPP
