#ifndef DIGITALINPUTOUTPUTVALUEENTITY_HPP
#define DIGITALINPUTOUTPUTVALUEENTITY_HPP

#include "digitalinputvalueentity.hpp"
#include "digitaloutputvalueentity.hpp"



namespace talorion {

  class DigitalInputOutputValueEntity:public DigitalInputValueEntity, public DigitalOutputValueEntity
  {
  public:
    DigitalInputOutputValueEntity(EntityManager::EntityID en);
    virtual ~DigitalInputOutputValueEntity() = default;
  };

} // namespace talorion

#endif // DIGITALINPUTOUTPUTVALUEENTITY_HPP
