#ifndef ANALOGVALUEENTITY_HPP
#define ANALOGVALUEENTITY_HPP

#include "valueentity.hpp"

namespace talorion {

  class AnalogValueEntity:public ValueEntity
  {
  public:
    AnalogValueEntity(EntityManager::EntityID en);
    virtual ~AnalogValueEntity() = default;


  };

} // namespace talorion

#endif // ANALOGVALUEENTITY_HPP
