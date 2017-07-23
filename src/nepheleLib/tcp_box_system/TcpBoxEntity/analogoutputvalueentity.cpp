#include "analogoutputvalueentity.hpp"

namespace talorion {

  AnalogOutputValueEntity::AnalogOutputValueEntity(EntityManager::EntityID en):
    AnalogValueEntity(en)
  {

  }

  double AnalogOutputValueEntity::setMin() const
  {
    double ret = qQNaN();
    auto var = entMng().componentDataForEntity(SET_MIN_COMPONENT, entity());
    if(var.canConvert<double>()){
        ret = var.value<double>();
      }
    return ret;
  }

  double AnalogOutputValueEntity::setMax() const
  {
    double ret = qQNaN();
    auto var = entMng().componentDataForEntity(SET_MAX_COMPONENT, entity());
    if(var.canConvert<double>()){
        ret = var.value<double>();
      }
    return ret;
  }

  double AnalogOutputValueEntity::setVal() const
  {
    double ret = qQNaN();
    auto var = entMng().componentDataForEntity(ANALOG_SET_VALUE_COMPONENT, entity());
    if(var.canConvert<double>()){
        ret = var.value<double>();
      }
    return ret;
  }

} // namespace talorion
