#ifndef DIGITALINPUTVALUEENTITY_HPP
#define DIGITALINPUTVALUEENTITY_HPP

#include "valueentity.hpp"

namespace talorion {

  class DigitalInputValueEntity:public ValueEntity
  {
  public:
    DigitalInputValueEntity(EntityManager::EntityID en);
    virtual ~DigitalInputValueEntity() = default;

    bool actVal() const;

  };

} // namespace talorion

#endif // DIGITALINPUTVALUEENTITY_HPP
