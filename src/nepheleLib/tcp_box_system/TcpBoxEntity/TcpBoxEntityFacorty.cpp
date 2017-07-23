#include "TcpBoxEntityFacorty.hpp"
#include "valueentity.hpp"


#include "../backend_type.hpp"

#include <tcp_box_system/JsonRpcDriver/jsonrpcdriverutils.hpp>

namespace talorion {

  TcpBoxEntityFacorty::TcpBoxEntityFacorty(QObject *par) : QObject(par)
  {

  }

  bool TcpBoxEntityFacorty::isConfigured(EntityManager::EntityID box)
  {
    auto valid = emngr().isValid(box);
    if(valid == false)
      return false;

    auto exists = emngr().entityExists(box);
    if(exists == false)
      return false;

    auto name = emngr().componentDataForEntity(nameComponentId(), box);
    if(name.isValid() == false)
      return false;

    auto host = emngr().componentDataForEntity(ipAddressComponentId(), box);
    if(host.isValid() == false)
      return false;

    auto port = emngr().componentDataForEntity(portComponentId(), box);
    if(port.isValid() == false)
      return false;

    auto boxId = emngr().componentDataForEntity(boxIdComponentId(), box);
    if(boxId.isValid() == false)
      return false;

    auto backend = emngr().componentDataForEntity(backendComponentId(), box);
    if(backend.isValid() == false)
      return false;

    bool ok = false;
    ok = name.canConvert<QString>();
    if(ok == false)
      return false;

    ok = host.canConvert<QString>();
    if(ok == false)
      return false;

    ok = port.canConvert<quint16>();
    if(ok == false)
      return false;

    ok = port.canConvert<int>();
    if(ok == false)
      return false;

    auto name_val = name.value<QString>();
    if(name_val.isEmpty() || name_val.isNull())
      return false;

    auto host_val = host.value<QString>();
    if(host_val.isEmpty() || host_val.isNull())
      return false;

//    auto port_val = port.value<quint16>();
//    if(port_val < 0 || port_val > 0xFFFF)
//      return false;

    auto backend_val = backend.value<backend_type>();
    if(backend_val < backend_type::UIBK_ECMD_FC || backend_val >= backend_type::COUNT)
      return false;

    return true;

  }

  QList<EntityManager::EntityID> TcpBoxEntityFacorty::getMethodsOfBox(EntityManager::EntityID box)
  {
    QList<EntityManager::EntityID> lst;
    if(emngr().get_all_tcpBoxes().contains(box) == false)
      return lst;

    auto chldr = emngr().getChildren(box);
    auto uid = emngr().get_JsonMethodDescriptor_uid();
    foreach (auto ch, chldr) {
        if(emngr().get_serialVersionUID_component(ch) == uid){
            lst.append(ch);
          }
      }
    return lst;
  }

  QList<EntityManager::EntityID> TcpBoxEntityFacorty::getValuesOfBox(EntityManager::EntityID box)
  {
    QList<EntityManager::EntityID> lst;
    if(emngr().get_all_tcpBoxes().contains(box) == false)
      return lst;

    auto chldr = emngr().getChildren(box);
    foreach (auto ch, chldr) {
        if(isValue(ch)){
            lst.append(ch);
          }
      }
    return lst;
  }

  void TcpBoxEntityFacorty::addMethodToBox(EntityManager::EntityID box_id, EntityManager::EntityID meth_id)
  {
    if(emngr().get_all_tcpBoxes().contains(box_id) == false)
      return;

    if(emngr().getChildren(box_id).contains(meth_id))
      return;

    foreach (auto child, getMethodsOfBox(box_id)) {
        if(JsonRpcDriverUtils::compareMethods(meth_id, child))
          return;
      }

    emngr().addChild(box_id, meth_id);
  }

  EntityManager::EntityID TcpBoxEntityFacorty::getAOValue(EntityManager::EntityID box, int valid)
  {
    auto uid = emngr().get_AnalogOutputValue_uid();
    auto chldr = emngr().getChildren(box);
    foreach (auto ch, chldr) {
        if(emngr().get_serialVersionUID_component(ch) != uid)
          continue;
        if(emngr().get_id_component(ch) == valid)
          return ch;
      }
    return EntityManager::invalid_id;

  }

  EntityManager::EntityID TcpBoxEntityFacorty::getAIValue(EntityManager::EntityID box, int valid)
  {
    auto uid = emngr().get_AnalogInputValue_uid();
    auto chldr = emngr().getChildren(box);
    foreach (auto ch, chldr) {
        if(emngr().get_serialVersionUID_component(ch) != uid)
          continue;
        if(emngr().get_id_component(ch) == valid)
          return ch;
      }
    return EntityManager::invalid_id;
  }

  EntityManager::EntityID TcpBoxEntityFacorty::getAIOValue(EntityManager::EntityID box, int valid)
  {
    auto uid = emngr().get_AnalogValue_uid();
    auto chldr = emngr().getChildren(box);
    foreach (auto ch, chldr) {
        if(emngr().get_serialVersionUID_component(ch) != uid)
          continue;
        if(emngr().get_id_component(ch) == valid)
          return ch;
      }
    return EntityManager::invalid_id;
  }

  EntityManager::EntityID TcpBoxEntityFacorty::getDOValue(EntityManager::EntityID box, int valid)
  {
    auto uid = emngr().get_DigitalOutputValue_uid();
    auto chldr = emngr().getChildren(box);
    foreach (auto ch, chldr) {
        if(emngr().get_serialVersionUID_component(ch) != uid)
          continue;
        if(emngr().get_id_component(ch) == valid)
          return ch;
      }
    return EntityManager::invalid_id;
  }

  EntityManager::EntityID TcpBoxEntityFacorty::getDIValue(EntityManager::EntityID box, int valid)
  {
    auto uid = emngr().get_DigitalInputValue_uid();
    auto chldr = emngr().getChildren(box);
    foreach (auto ch, chldr) {
        if(emngr().get_serialVersionUID_component(ch) != uid)
          continue;
        if(emngr().get_id_component(ch) == valid)
          return ch;
      }
    return EntityManager::invalid_id;
  }

  EntityManager::EntityID TcpBoxEntityFacorty::getDIOValue(EntityManager::EntityID box, int valid)
  {
    auto uid = emngr().get_DigitalValue_uid();
    auto chldr = emngr().getChildren(box);
    foreach (auto ch, chldr) {
        if(emngr().get_serialVersionUID_component(ch) != uid)
          continue;
        if(emngr().get_id_component(ch) == valid)
          return ch;
      }
    return EntityManager::invalid_id;
  }

  EntityManager::EntityID TcpBoxEntityFacorty::getValue(EntityManager::EntityID box, int valid)
  {
    auto values = getValuesOfBox(box);
    foreach(auto val, values){
        if(emngr().get_id_component(val) == valid)
          return val;
      }
    return EntityManager::invalid_id;
  }

  ValueType TcpBoxEntityFacorty::getValueType(EntityManager::EntityID val)
  {
    if(EntityManager::isValid(val) == false)
      return ValueType::VT_NONE;

    ValueEntity ve(val);
    auto uuid = ve.serialVersionUid();
    if(uuid == emngr().get_DigitalInputValue_uid() )
      return ValueType::VT_DIGITAL;

    if(uuid == emngr().get_DigitalOutputValue_uid() )
      return ValueType::VT_DIGITAL;

    if(uuid == emngr().get_DigitalValue_uid() )
      return ValueType::VT_DIGITAL;

    if(uuid == emngr().get_AnalogInputValue_uid())
      return ValueType::VT_ANALOG;

    if(uuid == emngr().get_AnalogOutputValue_uid())
      return ValueType::VT_ANALOG;

    if(uuid == emngr().get_AnalogValue_uid())
      return ValueType::VT_ANALOG;

    return ValueType::VT_NONE;
  }

  ValueMode TcpBoxEntityFacorty::getValueMode(EntityManager::EntityID val)
  {
    if(EntityManager::isValid(val) == false)
      return ValueMode::VM_NONE;

    ValueEntity ve(val);
    auto uuid = ve.serialVersionUid();
    if(uuid == emngr().get_DigitalInputValue_uid() )
      return ValueMode::VM_INPUT;

    if(uuid == emngr().get_DigitalOutputValue_uid() )
      return ValueMode::VM_OUTPUT;

    if(uuid == emngr().get_DigitalValue_uid() )
      return ValueMode::VM_INPUTOUTPUT;

    if(uuid == emngr().get_AnalogInputValue_uid())
      return ValueMode::VM_INPUT;

    if(uuid == emngr().get_AnalogOutputValue_uid())
      return ValueMode::VM_OUTPUT;

    if(uuid == emngr().get_AnalogValue_uid())
      return ValueMode::VM_INPUTOUTPUT;

    return ValueMode::VM_NONE;
  }

  bool TcpBoxEntityFacorty::isValue(EntityManager::EntityID entity)
  {
    if(EntityManager::isValid(entity) == false)
      return false;

    auto uid = emngr().get_serialVersionUID_component(entity);

    if(uid == emngr().get_AnalogValue_uid())
      return true;

    if(uid == emngr().get_AnalogInputValue_uid())
      return true;

    if(uid == emngr().get_AnalogOutputValue_uid())
      return true;

    if(uid == emngr().get_DigitalInputValue_uid())
      return true;

    if(uid == emngr().get_DigitalOutputValue_uid())
      return true;

    if(uid == emngr().get_DigitalValue_uid())
      return true;

    return false;

  }

} // namespace talorion
