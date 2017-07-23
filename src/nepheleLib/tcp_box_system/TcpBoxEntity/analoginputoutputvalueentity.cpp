#include "analoginputoutputvalueentity.hpp"

namespace talorion {

  AnalogInputOutputValueEntity::AnalogInputOutputValueEntity(EntityManager::EntityID en):
    AnalogInputValueEntity(en),
    AnalogOutputValueEntity(en)
  {

  }

//  bool AnalogInputOutputValueEntity::isValid() const
//  {
//    if(EntityManager::isValid(entity()) == false)
//      return false;

//    auto uuid = serialVersionUid();
//    if(entMng.get_AnalogValue_uid() != uuid)
//      return false;

//    return true;
//  }

} // namespace talorion
