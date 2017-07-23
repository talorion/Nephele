#include "analoginputvalueentity.hpp"

namespace talorion {


  AnalogInputValueEntity::AnalogInputValueEntity(EntityManager::EntityID en):
    AnalogValueEntity(en)
  {

  }

  double AnalogInputValueEntity::actMin() const
  {
    double ret = qQNaN();
    auto var = entMng().componentDataForEntity(ACT_MIN_COMPONENT, entity());
    if(var.canConvert<double>()){
        ret = var.value<double>();
      }
    return ret;
  }

  double AnalogInputValueEntity::actMax() const
  {
    double ret = qQNaN();
    auto var = entMng().componentDataForEntity(ACT_MAX_COMPONENT, entity());
    if(var.canConvert<double>()){
        ret = var.value<double>();
      }
    return ret;
  }

  double AnalogInputValueEntity::actVal() const
  {
    double ret = qQNaN();
    auto var = entMng().componentDataForEntity(ANALOG_ACT_VALUE_COMPONENT, entity());
    if(var.canConvert<double>()){
        ret = var.value<double>();
      }
    return ret;
  }

} // namespace talorion
