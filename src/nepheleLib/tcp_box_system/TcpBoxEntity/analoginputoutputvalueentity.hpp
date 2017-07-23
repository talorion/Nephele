#ifndef ANALOGINPUTOUTPUTVALUEENTITY_HPP
#define ANALOGINPUTOUTPUTVALUEENTITY_HPP

#include "analoginputvalueentity.hpp"
#include "analogoutputvalueentity.hpp"



namespace talorion {

  class AnalogInputOutputValueEntity:public AnalogInputValueEntity, public AnalogOutputValueEntity
  {
  public:
    AnalogInputOutputValueEntity(EntityManager::EntityID en);
    virtual ~AnalogInputOutputValueEntity() = default;

    //bool isValid()const;
  };

} // namespace talorion

#endif // ANALOGINPUTOUTPUTVALUEENTITY_HPP
