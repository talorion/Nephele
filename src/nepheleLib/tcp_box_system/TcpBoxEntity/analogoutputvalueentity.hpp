#ifndef ANALOGOUTPUTVALUEENTITY_HPP
#define ANALOGOUTPUTVALUEENTITY_HPP

#include "analogvalueentity.hpp"

namespace talorion {

  class AnalogOutputValueEntity:public AnalogValueEntity
  {
  public:
    AnalogOutputValueEntity(EntityManager::EntityID en);
    virtual ~AnalogOutputValueEntity() = default;

    double setMin() const;
    double setMax() const;
    double setVal() const;

  };

} // namespace talorion

#endif // ANALOGOUTPUTVALUEENTITY_HPP
