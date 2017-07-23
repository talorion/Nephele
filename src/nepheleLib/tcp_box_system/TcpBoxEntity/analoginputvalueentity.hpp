#ifndef ANALOGINPUTVALUEENTITY_HPP
#define ANALOGINPUTVALUEENTITY_HPP

#include "analogvalueentity.hpp"

namespace talorion {

  class AnalogInputValueEntity:public AnalogValueEntity
  {
  public:
    AnalogInputValueEntity(EntityManager::EntityID en);
    virtual ~AnalogInputValueEntity() = default;

    double actMin() const;
    double actMax() const;
    double actVal() const;


  };

} // namespace talorion

#endif // ANALOGINPUTVALUEENTITY_HPP
