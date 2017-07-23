#include "digitalinputvalueentity.hpp"

namespace talorion {

  DigitalInputValueEntity::DigitalInputValueEntity(EntityManager::EntityID en)
    :ValueEntity(en)
  {

  }

  bool DigitalInputValueEntity::actVal() const
  {
    int ret = 0;
    auto var = entMng().componentDataForEntity(DIGITAL_ACT_VALUE_COMPONENT, entity());
    if(var.canConvert<int>()){
        ret = var.value<int>();
      }
    return ret != 0;
  }

} // namespace talorion
