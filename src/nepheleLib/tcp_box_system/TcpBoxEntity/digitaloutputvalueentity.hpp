#ifndef DIGITALOUTPUTVALUEENTITY_HPP
#define DIGITALOUTPUTVALUEENTITY_HPP

#include "valueentity.hpp"

namespace talorion {

  class DigitalOutputValueEntity:public ValueEntity
  {
  public:
    DigitalOutputValueEntity(EntityManager::EntityID en);
    virtual ~DigitalOutputValueEntity() = default;

    bool setVal() const;

  };

} // namespace talorion

#endif // DIGITALOUTPUTVALUEENTITY_HPP
