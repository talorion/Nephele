#include "digitaloutputvalueentity.hpp"



namespace talorion {

  DigitalOutputValueEntity::DigitalOutputValueEntity(EntityManager::EntityID en):
    ValueEntity(en)
  {

  }

  bool DigitalOutputValueEntity::setVal() const
  {
    int ret = 0;
    auto var = entMng().componentDataForEntity(DIGITAL_SET_VALUE_COMPONENT, entity());
    if(var.canConvert<int>()){
        ret = var.value<int>();
      }
    return ret != 0;
  }

} // namespace talorion
